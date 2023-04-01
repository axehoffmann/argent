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
        template <typename... Q>
        void Query(void(*callback)(QueryResult<Q>...))
        {
            std::vector<ComponentTypeID> search;
            (search.push_back(ag::Component::GetID<Q>()), ...);

            std::vector<ag::ArchetypeCollection*> matches;

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

        void AddArchetype(ag::ArchetypeCollection* arch);

    private:
        std::vector<ag::ArchetypeCollection*> archetypes;
    };
}