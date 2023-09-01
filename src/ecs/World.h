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
        std::shared_ptr<ag::ArchetypeCollection> FindArchetype(const std::vector<ComponentTypeID>& set) const
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

        void SpawnEntity(const std::vector<ComponentTypeID>& set, const std::vector<Component>& components)
        {
            auto arch = FindArchetype(set);
            arch->SpawnEntity(components);
        }

        void SpawnEntity(std::shared_ptr<ArchetypeCollection> arch, const std::vector<Component>& components)
        {
            arch->SpawnEntity(components);
        }

        /**
        * Adds an archetype to the world collection.
        * @param arch The archetype to add. 
        */
        void AddArchetype(ag::ArchetypeCollection* arch);

        void InitialiseQuery(ag::IQuery* query);


        void DeleteQuery(ag::IQuery* query)
        {
            queries.erase(std::remove(queries.begin(), queries.end(), query), queries.end());
        }

        ~World();

    private:
        std::vector<std::shared_ptr<ag::ArchetypeCollection>> archetypes;
        std::vector<ag::IQuery*> queries;
    };
}