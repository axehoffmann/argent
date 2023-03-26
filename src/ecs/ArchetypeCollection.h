#pragma once

#include "ECSTypes.h"
#include "Component.h"

#include <unordered_map>
#include <algorithm>
#include <atomic>

namespace ag
{
	struct EntityInfo
	{
		EntityID ID;
		EntityID ParentID;
	};

	class ArchetypeCollection
	{
	public:
		template <typename... Cs>
		EntityID SpawnEntity(Cs... component)
		{
			return InstantiateEntity(0, component...);
		}

		template <typename... Cs>
		EntityID SpawnChild(EntityID parent, Cs... component)
		{
			return InstantiateEntity(parent, component...);
		}

		template <typename C>
		C* GetComponent(size_t i)
		{
			if (i >= entities.size())
			{
				return nullptr;
			}
			
			size_t stride = sizeof(C);
			// The component array that stores this component type
			auto ci = std::find(ComponentTypes.begin(), ComponentTypes.end(), ag::Component::GetID<C>()) - ComponentTypes.begin();
            
			// Return a pointer to the component
			return (C*)(&data[ci].at(i * stride));
		}

		ArchetypeID GetID()
		{
			return ID;
		}

		size_t GetIndexByID(EntityID id);

		size_t GetEntityCount() { return entities.size(); }

		void ResolveBuffers();

		void DestroyEntity(size_t index);
		void DestroyEntityByID(EntityID id);

		ArchetypeCollection(ComponentSet components);
		~ArchetypeCollection();

		static void RegisterArchetype(ArchetypeCollection* archetype);
		static void DeregisterArchetype(ArchetypeID id);
		static ArchetypeCollection* GetArchetypeFromEntityID(EntityID id);


	private:

		template <typename... Cs>
		EntityID InstantiateEntity(EntityID parentID, Cs... component)
		{
			EntityID newEntityID = GetNextID();
			EntityInfo entityData;
			entityData.ID = newEntityID;
			entityData.ParentID = parentID;
			entitiesToSpawn.push_back(entityData);

			int i = 0;
			// We assume the components are in the order matching the archetype and add them to each component array (in the spawn buffer)
			(AddComponent((byte*)&component, i++, sizeof(component), spawnBuffer), ...);

			return newEntityID;
		}

		ArchetypeID ID;
		// The component types that make up this archetype
		ComponentSet ComponentTypes;
		// The entity data stored in this archetype. Each component array is a byte array of a specific component type.
		ComponentArray* data;

		EntityID GetNextID();
		// The ID of the next entity to spawn
		EntityID NextEntityID;

		// Entity creation/destruction buffers
		ComponentArray* spawnBuffer;
		std::vector<EntityInfo> entitiesToSpawn;
		void ResolveSpawnBuffer();
		std::vector<size_t> entitiesToDestroy;
		void ResolveDestroyBuffer();

		// Core entity data (a component that every entity has)
		std::vector<EntityInfo> entities;

		void AddComponent(byte* bytes, int i, int n, ComponentArray* target);

		static std::unordered_map<ArchetypeID, ArchetypeCollection*> archetypes;
		static std::atomic<ArchetypeID> nextArchetypeID;
	};
}

