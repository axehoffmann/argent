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
    public:
        /**
        * Called for all Queries at the end of each update to cache new ranges.
        */
        void UpdateRanges()
        {
            for (size_t i = 0; i < matches.size(); i++)
            {
                ranges[i] = matches[i]->GetEntityCount();
            }
        }

    protected:
        /**
        * Private structure used to contain Entity location after a search.
        */
        struct EntityLocation
        {
            std::shared_ptr<ag::ArchetypeCollection> archetype;
            size_t index;
            EntityLocation(std::shared_ptr<ag::ArchetypeCollection> a, size_t i) : archetype(a), index(i) {}
        };

        EntityLocation EntityLocationFromIndex(size_t index)
        {
            // Binary search through ranges to find the archetype that aligns with this index
            /// TODO: Test for safety
            /// TODO: Benchmark compare with old iterative algorithm.
            size_t lo = 0;
            size_t hi = ranges.size() - 1;

            while (lo <= hi)
            {
                size_t mid = (lo + hi) / 2;

                if (index >= ranges[mid] && index < ranges[mid + 1])
                    return EntityLocation(matches[mid], index - std::accumulate(matches.begin(), matches.begin() + mid, 0));

                else if (index < ranges[mid])
                    hi = mid - 1;

                else if (index >= ranges[mid + 1])
                    lo = mid + 1;
            }

            throw std::out_of_range("Could not find entity location from Query index " + std::to_string(index));
        }

        std::vector<std::shared_ptr<ArchetypeCollection>> matches;
        std::vector<size_t> ranges;
    };

    template <typename... ComponentTypes>
    class Query : public IQuery 
    {
    public:

        template <typename C>
        C& operator[](size_t i) { return At<C>(i) }

        template <typename C>
        C& At(size_t i)
        {
            // Type parameter for accessor MUST be one of the types included in the Query.
            static_assert(std::is_same_v<C, ComponentTypes>);

            EntityLocation loc = EntityLocationFromIndex(i);
            
            return loc.archetype->GetComponent<C>(i);
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