#pragma once

#include "ECSTypes.h"
#include "ArchetypeCollection.h"

#include <vector>
#include <tuple>
#include <algorithm>
#include <type_traits>
#include <numeric>

namespace ag
{
    /// TODO: New Architecture:
    ///     - World stores a list of Queries
    ///     - Each Query matches a set of Archetypes
    ///     - Whenever a new Archetype is created (infrequently), it is added to all matching Queries.
    ///     - Systems interact ONLY with its Queries, decouple System from World
    ///     - Identical queries should be merged by the World.
    ///     - No longer need callback functions
    /// 
    /// Usage:
    ///     System initialises all its queries on startup, via something like Query<A,B,C> myQuery = Query::Create<A,B,C>();
    ///     New queries should not be created post-initialisation. (But would probably work anyway).
    ///     No callback functions means all this data can just be accessed straight in update funcs
    ///
    class IQuery
    {
    protected:
        std::vector<std::shared_ptr<ag::ArchetypeCollection>> matches;
    };

    template <typename... ComponentTypes>
    class Query : public IQuery 
    {
    public:
        class Iterator
        {
            Query::Entity operator*()
            {
                return Query::Entity(currentArchetype.get(), index);
            }

            bool operator!=(const Iterator& other)
            {
                return currentArchetype != other.currentArchetype || index != other.index;
            }

            Iterator& operator++()
            {
                index++;

                if (index >= currentArchetypeSize)
                {
                    index = 0;
                    archIndex++;
                    currentArchetype = query->matches[archIndex];
                    currentArchetypeSize = currentArchetype->GetEntityCount();
                }

                return *this;
            }


        private:
            Iterator(Query<ComponentTypes...>* q, size_t arch, size_t i) : 
                query(q), archIndex(arch), index(i),
                currentArchetype(q->matches[arch]), 
                currentArchetypeSize(currentArchetype->GetEntityCount()) {}

            std::shared_ptr<ag::ArchetypeCollection> currentArchetype;
            size_t archIndex; // Index of the current Archetype within the parent Query's 'matches' array.
            size_t index; // Index of the iterator WITHIN the current archetype. Once this exceeds the current Archetype's bounds, we move to the next archetype in the query.
            size_t currentArchetypeSize; // We cache this so we don't go through 3 function calls to get the archetype's size every time we iterate.
            Query<ComponentTypes...>* const query;
        };

        struct Entity
        {
            template <typename C>
            C& operator->()
            {
                static_assert(std::is_same_v<C, ComponentTypes>);
                return archetype->GetComponent<C>(index);
            }

            EntityInfo& Info()
            {
                return archetype->GetEntityInfo(index);
            }

            // This should not be copyable and cannot persist outside of a Query iteration
            // because after entities are added or deleted this may no longer point to
            // the same entity.
            Query::Entity(const Query::Entity&) = delete;
            Query::Entity& operator=(const Query::Entity&) = delete;

        private:
            Entity(ag::ArchetypeCollection* a, size_t i) : archetype(a), index(i) {}

            const ag::ArchetypeCollection* archetype;
            const size_t index;
        };

        Iterator begin()
        {
            return Iterator(this, 0, 0);
        }

        Iterator end()
        {
            return Iterator(this, matches.size() - 1, matches.back()->GetEntityCount());
        }

    private:
    };

    /**
    * Indexes into a subset of ArchetypeCollections that match a query.
    * Each QueryResult can examine one component type determined by its template parameter.
    */
    template <typename ComponentType>
    struct QueryResult
    {
        /// TODO: alternatives to friend?
        friend class World;
    public:

        size_t Length()
        {
            return length;
        }

        /**
        * Returns the EntityInfo block from the specified entity
        * @param index The index of the entity within this query to fetch
        */
        std::optional<ag::EntityInfo> GetInfo(size_t index)
        {
            if (index >= length)
                return std::optional<ag::EntityInfo>{};

            auto targetEntityLocation = FindArchetypeAndLocalIndex(index);
            
            if (std::get<std::shared_ptr<ag::ArchetypeCollection>>(targetEntityLocation) == nullptr || std::get<size_t>(targetEntityLocation) < 0)
                return std::optional<ag::EntityInfo>{};
            
            return std::get<std::shared_ptr<ag::ArchetypeCollection>>(targetEntityLocation)->GetEntityInfo(std::get<size_t>(targetEntityLocation));
        }

        /// TODO: Repeated code.
        ComponentType& At(size_t index)
        {
            if (index >= length)
                throw std::out_of_range("QueryResult index out of range");

            auto targetEntityLocation = FindArchetypeAndLocalIndex(index);

            if (std::get<std::shared_ptr<ag::ArchetypeCollection>>(targetEntityLocation) == nullptr || std::get<size_t>(targetEntityLocation) < 0)
                throw std::runtime_error("Query could not find a matching archetype");

            return std::get<std::shared_ptr<ag::ArchetypeCollection>>(targetEntityLocation)->GetComponent<ComponentType>(std::get<size_t>(targetEntityLocation));
        }

        ComponentType& operator[](size_t index)
        {
            return At(index);
        }

        /**
        * Returns the component from the specified entity
        * @param id The ID of the entity to fetch the component from
        */
        ComponentType& ByID(EntityID id)
        {
            ag::ArchetypeCollection* match = ag::ArchetypeCollection::GetArchetypeFromEntityID(id);

            /// TODO: Cannot return an optional reference. Find an alternative for what to do when not found.
            // Can only access an entity by ID if it is included in this query
            if (!std::count(matchingCollections.begin(), matchingCollections.end(), match))
                return nullptr;
        
            return At(match->GetIndexByID(id));
        }

    private:

        /// TODO: should use weak_ptrs
        QueryResult(std::vector<std::shared_ptr<ag::ArchetypeCollection>> matches)
        {
            matchingCollections = matches;

            length = 0;
            for (size_t i = 0; i < matchingCollections.size(); i++)
            {
                length += matchingCollections[i]->GetEntityCount();
            }
        }

        std::vector<std::shared_ptr<ag::ArchetypeCollection>> matchingCollections;
        size_t length;

        /**
         * Finds the Archetype in matchingCollections, and the index of the entity within that archetype.
         * @param index The indexer into the QueryResult.
         * @return An std::tuple of the Archetype in matchingCollections, and the local entity index.
        */
        std::tuple<std::shared_ptr<ag::ArchetypeCollection>, size_t> FindArchetypeAndLocalIndex(size_t index)
        {
            /// TODO: could probably optimise by caching ranges in constructor? The ranges would have to update once the buffers are resolved though
            size_t localIndex = index;

            for (size_t i = 0; i < matchingCollections.size(); i++)
            {
                std::shared_ptr<ag::ArchetypeCollection>& a = matchingCollections[i];
                if (localIndex >= a->GetEntityCount())
                    localIndex -= a->GetEntityCount();
                else
                    return std::tuple<std::shared_ptr<ag::ArchetypeCollection>, size_t>{a, localIndex};
            }

            throw std::runtime_error("QueryResult was unable to find an ArchetypeCollection");
        }
    };
}