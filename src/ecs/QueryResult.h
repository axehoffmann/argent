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
        QueryResult(std::vector<ag::ArchetypeCollection*> matches)
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

        ComponentType* operator[](size_t index)
        {
            if (index >= length)
                return nullptr;

            std::tuple<ag::ArchetypeCollection*, size_t> find = FindArchetypeAndLocalIndex(index);
            
            if (std::get<ag::ArchetypeCollection*>(find) == nullptr || std::get<size_t>(find) < 0)
                return nullptr;

            return std::get<ag::ArchetypeCollection*>(find)->GetComponent<ComponentType>(std::get<size_t>(find));
        }

        ComponentType* ByID(EntityID id)
        {
            ag::ArchetypeCollection* match = ag::ArchetypeCollection::GetArchetypeFromEntityID(id);

            // Can only access an entity by ID if it is included in this query
            if (!std::count(matchingCollections.begin(), matchingCollections.end(), match))
                return nullptr;
        
            return match->GetIndexByID(id);
        }

        /**
         * Public access to parameter type
        */
        typedef ComponentType cType;
    private:
        std::vector<ag::ArchetypeCollection*> matchingCollections;
        size_t length;

        /**
         * Finds the Archetype in matchingCollections, and the index of the entity within that archetype.
         * @param index The indexer into the QueryResult.
         * @return An std::tuple of the Archetype in matchingCollections, and the local entity index.
        */
        std::tuple<ag::ArchetypeCollection*, size_t> FindArchetypeAndLocalIndex(size_t index)
        {
            /// TODO: could probably optimise by caching ranges in constructor
            size_t localIndex = index;

            for (size_t i = 0; i < matchingCollections.size(); i++)
            {
                ag::ArchetypeCollection* a = matchingCollections[i];
                if (localIndex >= a->GetEntityCount())
                    localIndex -= a->GetEntityCount();
                else
                    return std::tuple<ag::ArchetypeCollection*, size_t>{a, localIndex};
            }

            /// TODO: throw out of range exceptions?
            return std::tuple<ag::ArchetypeCollection*, size_t>{nullptr, -1};
        }
    };
}