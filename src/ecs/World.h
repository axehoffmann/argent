#pragma once

#include "ECSTypes.h"
#include "ArchetypeCollection.h"
#include "Query.h"

#include <memory>
#include <vector>
#include <algorithm>
#include <functional>

namespace ag
{
    class IQuery;
    class World
    {
    public:

        /**
        * Finds an archetype who's set of components matches the query set exactly. If no match is found, a new ArchetypeCollection is created and returned.
        * @param set    A vector of ComponentTypeIDs (expected to be sorted ascendingly) to query
        * @returns      An ArchetypeCollection with an exactly matching component set.
        */
        std::shared_ptr<ag::ArchetypeCollection> FindArchetype(std::vector<ComponentTypeID>& set)
        {
            /// TODO: benchmark... maybe could use a search tree

            for (size_t i = 0; i < archetypes.size(); i++)
            {
                std::vector<ComponentTypeID> currentSet = archetypes[i]->GetComponentSet();

                if (set == currentSet)
                    return archetypes[i];
            }

            /// TODO: should create an archetype if it doesn't already exist
            throw std::exception("NOT IMPLEMENTED");
        }

        /**
        * Adds an archetype to the world collection.
        * @param arch The archetype to add. 
        */
        void AddArchetype(ag::ArchetypeCollection* arch);

        void InitialiseQuery(ag::IQuery* query);


        void DeleteQuery(ag::IQuery* query)
        {
            auto iter = std::find(queries.begin(), queries.end(), query);
            if (iter != queries.end())
                queries.erase(iter);
        }

        ~World();

    private:
        std::vector<std::shared_ptr<ag::ArchetypeCollection>> archetypes;
        std::vector<ag::IQuery*> queries;
    };
}