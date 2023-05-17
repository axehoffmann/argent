#pragma once

#include "ECSTypes.h"
#include "ArchetypeCollection.h"
#include "QueryResult.h"

#include <vector>
#include <algorithm>
#include <functional>

namespace ag
{
    class World
    {
    public:
        /// TODO: check for issues with the way the callback is passed
        /**
        * Queries the world for all entities with the specified Component types
        * @param callback A callback function where the arguments QueryResult<A>, QueryResult<B>... specify the Components A, B... to search for.
        */
        template <typename... Q>
        void Query(std::function<void(QueryResult<Q>...)>& callback)
        {
            std::vector<ComponentTypeID> search;
            (search.push_back(ag::Component::GetID<Q>()), ...);

            std::vector<ag::ArchetypeCollection*> matches;

            /// TODO: benchmark... maybe could use a search tree
            for (size_t i = 0; i < archetypes.size(); i++)
            {
                std::vector<ComponentTypeID> currentSet = archetypes[i]->GetComponentSet();

                if (std::includes(currentSet.begin(), currentSet.end(), search.begin(), search.end()))
                    matches.push_back(archetypes[i]);
            }
            
            if (matches.size() == 0)
                return;

            callback( QueryResult<Q>(matches)... );
        }

        /**
        * Adds an archetype to the world collection.
        * @param arch The archetype to add. 
        */
        void AddArchetype(ag::ArchetypeCollection* arch);

        ~World();

    private:
        std::vector<ag::ArchetypeCollection*> archetypes;
    };
}