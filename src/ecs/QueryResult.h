#pragma once

#include "ECSTypes.h"
#include "ArchetypeCollection.h"

#include <vector>
#include <tuple>
#include <algorithm>

namespace ag
{
    template <typename ComponentType>
    struct QueryResult
    {
    public:
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

            std::tuple<ag::ArchetypeCollection*, size_t> targetEntityLocation = FindArchetypeAndLocalIndex(index);
            
            if (std::get<ag::ArchetypeCollection*>(targetEntityLocation) == nullptr || std::get<size_t>(targetEntityLocation) < 0)
                return std::optional<ag::EntityInfo>{};
            
            return std::get<ag::ArchetypeCollection*>(targetEntityLocation)->GetEntityInfo(std::get<size_t>(targetEntityLocation));
        }

        /// TODO: Repeated code.
        ComponentType& At(size_t index)
        {
            if (index >= length)
                throw std::out_of_range("QueryResult index out of range");

            std::tuple<ag::ArchetypeCollection*, size_t> targetEntityLocation = FindArchetypeAndLocalIndex(index);

            if (std::get<ag::ArchetypeCollection*>(targetEntityLocation) == nullptr || std::get<size_t>(targetEntityLocation) < 0)
                throw std::runtime_error("Query could not find a matching archetype");

            return std::get<ag::ArchetypeCollection*>(targetEntityLocation)->GetComponent<ComponentType>(std::get<size_t>(targetEntityLocation));
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

            // Can only access an entity by ID if it is included in this query
            if (!std::count(matchingCollections.begin(), matchingCollections.end(), match))
                return nullptr;
        
            return At(match->GetIndexByID(id));
        }

        /**
         * Public access to parameter type
        */
        typedef ComponentType cType;
    private:
        std::vector<std::shared_ptr<ag::ArchetypeCollection>> matchingCollections;
        size_t length;

        /**
         * Finds the Archetype in matchingCollections, and the index of the entity within that archetype.
         * @param index The indexer into the QueryResult.
         * @return An std::tuple of the Archetype in matchingCollections, and the local entity index.
        */
        std::tuple<ag::ArchetypeCollection*, size_t> FindArchetypeAndLocalIndex(size_t index)
        {
            /// TODO: could probably optimise by caching ranges in constructor? The ranges would have to update once the buffers are resolved though
            size_t localIndex = index;

            for (size_t i = 0; i < matchingCollections.size(); i++)
            {
                ag::ArchetypeCollection* a = matchingCollections[i];
                if (localIndex >= a->GetEntityCount())
                    localIndex -= a->GetEntityCount();
                else
                    return std::tuple<ag::ArchetypeCollection*, size_t>{a, localIndex};
            }

            throw std::runtime_error("QueryResult was unable to find an ArchetypeCollection");
        }
    };
}