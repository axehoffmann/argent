#pragma once

#include "ECSTypes.h"
#include "Component.h"

#include <unordered_map>
#include <algorithm>
#include <atomic>
#include <string>
#include <optional>

#include <stdexcept>

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
		 * @param name A name for the entity to spawn. This does not have to be unique.
		 * @param component The list of components that comprise the entity. This should be sorted by Component IDs.
		 * @return The ID of the newly created entity.
		*/
		template <typename... Cs>
		EntityID SpawnEntity(std::string name, Cs... component)
		{
			/// TODO: This may need to be mutexed
			return InstantiateEntity(name, component...);
		}

		/**
		* Adds an entity to the spawn buffer from a vector of type-erased components
		*/
		EntityID SpawnEntity(std::string name, std::vector<ag::Component>& components)
		{
			return InstantiateEntity(name, components);
		}

		/**
		 * Returns an EntityInfo struct from an index.
		 * @param i The index of the entity in the collection.
		 * @return A copy of the EntityInfo struct of the entity
		*/
		std::optional<EntityInfo> GetEntityInfo(size_t i)
		{
			if (i >= entities.size() || i < 0)
			{
				return std::optional<EntityInfo>();
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
		C& GetComponent(size_t i)
		{
			if (i >= entities.size() || i < 0) 
				throw std::out_of_range("ArchetypeCollection GetComponent index out of range");

			/// TODO: is this check too intensive to do a gazillion times a second? can we cache this some way idk
			if (std::find(ComponentTypes.begin(), ComponentTypes.end(), ComponentInfo::GetID<C>()) == ComponentTypes.end())
				throw std::runtime_error("Attempted to get component of type " + std::string(typeid(C).name()));

			// The component array that stores this component type
			/// TODO: should this be a hash map? benchmark
			auto ci = std::find(ComponentTypes.begin(), ComponentTypes.end(), ag::ComponentInfo::GetID<C>()) - ComponentTypes.begin();
            
			// Return a reference to the component
			return (C&)(data[ci].at(i * sizeof(C)));
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


		EntityID InstantiateEntity(std::string name, std::vector<ag::Component>& components)
		{
			EntityID newEntityID = GetNextID();
			EntityInfo entityData;
			entityData.ID = newEntityID;
			entityData.name = name;
			entitiesToSpawn.push_back(entityData);

			for (size_t i = 0; i < components.size(); i++)
			{
				AddComponent((byte*)components[i].GetRawData(), i, components[i].Size(), spawnBuffer);
			}

			return newEntityID;
		}

		ArchetypeID ID;
		// The component types that make up this archetype
		ComponentSet ComponentTypes;
		// The entity data stored in this archetype. Each component array is a byte array of a specific component type.
		std::vector<ComponentArray> data;

		EntityID GetNextID();
		// The ID of the next entity to spawn
		EntityID NextEntityID;

		// Entity creation/destruction buffers
		std::vector<ComponentArray> spawnBuffer;
		std::vector<EntityInfo> entitiesToSpawn;
		void ResolveSpawnBuffer();
		std::vector<size_t> entitiesToDestroy;
		void ResolveDestroyBuffer();

		// Core entity data (a component that every entity has)
		std::vector<EntityInfo> entities;

		void AddComponent(byte* bytes, int i, int n, std::vector<ComponentArray>& target);

		static void RegisterArchetype(ArchetypeCollection* archetype);
		static void DeregisterArchetype(ArchetypeID id);

		static std::unordered_map<ArchetypeID, ArchetypeCollection*> archetypes;
		static std::atomic<ArchetypeID> nextArchetypeID;
	};
}

