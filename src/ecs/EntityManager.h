#pragma once

#include "ArchetypeCollection.h"
#include <memory>

namespace ag
{
	class EntityManager
	{
	public:
		void SpawnEntity();
		void DestroyEntity();

		/**
		* Finds an archetype who's set of components matches the query set exactly. If no match is found, a new ArchetypeCollection is created and returned.
		* @param set    A vector of ComponentTypeIDs (expected to be sorted ascendingly) to query
		* @returns      An ArchetypeCollection with an exactly matching component set.
		*/
		std::shared_ptr<ArchetypeCollection> findArchetype(const ComponentSet& set)
		{
			/// TODO: benchmark... maybe could use a search tree

			for (size_t i = 0; i < archetypes.size(); i++)
			{
				std::vector<ComponentTypeID> currentSet = archetypes[i]->GetComponentSet();

				if (set == currentSet)
					return archetypes[i];
			}

			/// TODO: not synchronised
			std::shared_ptr<ArchetypeCollection> arch = std::make_shared<ArchetypeCollection>(set);
			archetypes.push_back(arch);
			id_to_archetype[arch->GetID()] = arch.get();
			return arch;
		}

		void SpawnEntity(const ComponentSet& set, const std::vector<Component>& components)
		{
			auto arch = findArchetype(set);
			arch->SpawnEntity(components);
		}

		void SpawnEntity(std::shared_ptr<ArchetypeCollection> arch, const std::vector<Component>& components)
		{
			arch->SpawnEntity(components);
		}
	
	private:
		std::vector<std::shared_ptr<ArchetypeCollection>> archetypes;
		std::unordered_map<ArchetypeID, ArchetypeCollection*> id_to_archetype;
	};
}