#pragma once

#include "ECSTypes.h"
#include "Component.h"

#include <unordered_map>
#include <algorithm>
#include <atomic>
#include <string>

namespace ag
{
	struct EntityInfo
	{
		/// TODO: Entity names should be stored in a map elsewhere so repeated names don't take up more memory
		std::string name;
		EntityID ID;
	};

	/**
	 * A collection that stores entities of a specific archetype, determined by the collection's ComponentTypes.
	*/
	class ArchetypeCollection
	{
	public:
		/**
		 * Adds an entity to the spawn buffer.
		 * @param component The list of components that comprise the entity. This should be sorted by Component IDs.
		 * @param name A name for the entity to spawn. This does not have to be unique.
		 * @return The ID of the newly created entity.
		*/
		template <typename... Cs>
		EntityID SpawnEntity(std::string name, Cs... component)
		{
			/// TODO: This may need to be mutexed
			return InstantiateEntity(name, component...);
		}

		/**
		 * Returns an EntityInfo struct from an index.
		 * @param i The index of the entity in the collection.
		 * @return A copy of the EntityInfo struct of the entity
		*/
		EntityInfo GetEntityInfo(size_t i)
		{
			/// TODO: return an optional?
			if (i >= entities.size() || i < 0)
			{
				return EntityInfo();
			}

			return entities[i];
		}


		/**
		 * Gets a pointer to a component from the collection.
		 * @param i The index of the entity in the collection.
		 * @tparam C The type of component to fetch.
		 * @return A pointer to the fetched component, or nullptr if the search was invalid.
		*/
		template <typename C>
		C* GetComponent(size_t i)
		{
			/// TODO: Checks for values < 0, and component types not included in the collection?
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

		size_t GetEntityCount() { return entities.size(); }

		/**
		 * Searches for the index of an entity in the collection by its ID.
		 * @param id The id of the entity to search for.
		 * @return The index of the found entity, or -1 if no entity was found.
		*/
		size_t GetIndexByID(EntityID id);

		/**
		 * Resolves the spawn and destroy buffers of the collection.
		*/
		void ResolveBuffers();

		/**
		 * Marks an entity to be destroyed.
		 * @param index The index of the entity to destroy.
		*/
		void DestroyEntity(size_t index);

		/**
		 * Marks an entity to be destroyed.
		 * @param id The ID of the entity to destroy.
		*/
		void DestroyEntityByID(EntityID id);

		ArchetypeCollection(ComponentSet components);
		~ArchetypeCollection();

		/**
		 * Finds the collection an entity belongs to by its ID.
		 * @param id The ID of the entity to find.
		 * @return A pointer to the collection the entity belongs to, or nullptr if a collection could not be found.
		*/
		static ArchetypeCollection* GetArchetypeFromEntityID(EntityID id);

		ComponentSet GetComponentSet();

	private:

		template <typename... Cs>
		EntityID InstantiateEntity(std::string name, Cs... component)
		{
			EntityID newEntityID = GetNextID();
			EntityInfo entityData;
			entityData.ID = newEntityID;
			entityData.name = name;
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

		static void RegisterArchetype(ArchetypeCollection* archetype);
		static void DeregisterArchetype(ArchetypeID id);

		static std::unordered_map<ArchetypeID, ArchetypeCollection*> archetypes;
		static std::atomic<ArchetypeID> nextArchetypeID;
	};
}

