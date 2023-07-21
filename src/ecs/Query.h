#pragma once

#include "ECSTypes.h"
#include "World.h"
#include "ArchetypeCollection.h"

#include <vector>
#include <tuple>
#include <algorithm>
#include <type_traits>
#include <numeric>

#include "debug/log/Log.h"

namespace ag
{
    class World;
    class IQuery
    {
    public:
        /**
        * Adds an archetype to this Query if it matches the Query's component set.
        * @param archetype A pointer to the archetype match candidate.
        */
        void AddIfMatch(std::shared_ptr<ag::ArchetypeCollection> archetype)
        {
            if (Matches(archetype->GetComponentSet()))
                matches.push_back(archetype);
        }

        IQuery(ag::World* w, const ComponentSet& set);

        /**
        * Unsubscribe from getting archetype updates
        */
        ~IQuery();

        /**
        * Calculates total count of matched entities.
        */
        size_t Size() const
        {
            return std::accumulate(matches.begin(), matches.end(), 0,
                [](size_t sum, std::shared_ptr<ag::ArchetypeCollection> b) {
                return sum + b->GetEntityCount();
            });
        }


    protected:
        bool Matches(const ComponentSet& set) const
        {
            return std::includes(set.begin(), set.end(), componentTypes.begin(), componentTypes.end());
        }

        std::vector<std::shared_ptr<ag::ArchetypeCollection>> matches;
        ag::World* world;

        /// TODO: consider a small-size optimised hash set implementation for component set comparisons. 
        /// Probably uneccesary in this case though. https://www.youtube.com/watch?v=vElZc6zSIXM
        ComponentSet componentTypes;
    };

    template <typename... Cs>
    class Query : public IQuery 
    {
        friend class ag::World;
    public:

        Query(ag::World* w) : IQuery(w, ag::ComponentInfo::GetComponentSet<Cs...>()) {}

        class Iterator
        {
            friend class Query;

        public:
            Query::Entity operator*()
            {
                return Query::Entity(currentArchetype.get(), index);
            }

            bool operator!=(const Iterator& other) const
            {
                return currentArchetype != other.currentArchetype || index != other.index;
            }

            Iterator& operator++()
            {
                index++;

                if (index >= currentArchetypeSize && archIndex + 1 < query->matches.size())
                {
                    index = 0;
                    archIndex++;
                    currentArchetype = query->matches[archIndex];
                    currentArchetypeSize = currentArchetype->GetEntityCount();
                }

                return *this;
            }


        private:
            Iterator(Query<Cs...>* q, size_t arch, size_t i) : 
                query(q), archIndex(arch), index(i),
                currentArchetype(q->matches[arch]),
                currentArchetypeSize(currentArchetype->GetEntityCount()) {}

            // Null Iterator for empty Queries
            Iterator(Query<Cs...>* q):
                query(q), archIndex(0), index(0),
                currentArchetype(),
                currentArchetypeSize(0) {}

            std::shared_ptr<ag::ArchetypeCollection> currentArchetype;
            size_t archIndex; // Index of the current Archetype within the parent Query's 'matches' array.
            size_t index; // Index of the iterator WITHIN the current archetype. Once this exceeds the current Archetype's bounds, we move to the next archetype in the query.
            size_t currentArchetypeSize; // We cache this so we don't go through 3 function calls to get the archetype's size every time we iterate.
            Query<Cs...>* query;
        };
        
        /**
        * An indexer object used to conveniently access the properties of entities in a query.
        */
        struct Entity
        {
            friend class Query::Iterator;
            friend class Query;

            /**
            * Get a reference to a component from this entity.
            * @tparam The type of component to get. Must be a type in the query.
            * @return A reference to the selected component
            */
            template <typename C>
            C& Get()
            {
                // Component we are getting must be one of the types from the Query this entity is from
                static_assert((std::is_same_v<C, Cs> || ...));
                return archetype->GetComponent<C>(index);
            }

            /**
            * Get a copy of the info struct of the entity.
            */
            EntityInfo Info() const
            {
                return archetype->GetEntityInfo(index);
            }

            /**
            * Mark the entity to be destroyed next update.
            */
            void Destroy() 
            {
                archetype->DestroyEntity(index);
            }

            // This should not be copyable and cannot persist outside of a Query iteration
            // because after entities are added or deleted this may no longer point to
            // the same entity.
            Query::Entity(const Query::Entity&) = delete;
            Query::Entity& operator=(const Query::Entity&) = delete;

        private:
            Entity(ag::ArchetypeCollection* a, size_t i) : archetype(a), index(i) {}

            ag::ArchetypeCollection* archetype;
            size_t index;
        };

        /**
        * Generates an Entity indexer referencing index 'i' of the Query.
        * @param i The index of the Entity to get within this Query's bounds
        * @return An entity indexer
        */
        Entity at(size_t i) const
        {
            // We won't bounds check as that is done in FindArchetypeAndLocalIndex and within the Archetype itself
            auto[archetype, localIndex] = FindArchetypeAndLocalIndex(i);

            return Entity(archetype.get(), localIndex);
        }

        Iterator begin()
        {
            if (matches.size() == 0)
                return Iterator(this);

            return Iterator(this, 0, 0);
        }

        Iterator end()
        {
            if (matches.size() == 0)
                return Iterator(this);

            return Iterator(this, matches.size() - 1, matches.back()->GetEntityCount());
        }

    private:
        /**
         * Finds the Archetype in matches, and the index of the entity within that archetype.
         * @param index The indexer into the Query.
         * @return An std::tuple of the aligned ArchetypeCollection, and the local entity index.
         */
        std::tuple<std::shared_ptr<ag::ArchetypeCollection>, size_t> FindArchetypeAndLocalIndex(size_t index) const
        {
            /// TODO: could probably optimise by caching ranges in constructor? The ranges would have to update once the buffers are resolved though
            size_t localIndex = index;

            for (size_t i = 0; i < matches.size(); i++)
            {
                std::shared_ptr<ag::ArchetypeCollection>& a = matches[i];
                if (localIndex >= a->GetEntityCount())
                    localIndex -= a->GetEntityCount();
                else
                    return std::tuple<std::shared_ptr<ag::ArchetypeCollection>, size_t>{a, localIndex};
            }

            Log::Error("Query was unable to find an ArchetypeCollection");
            throw std::runtime_error("QueryResult was unable to find an ArchetypeCollection");
        }
    };
}