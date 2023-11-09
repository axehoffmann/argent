#pragma once

#include "ECSTypes.h"
#include "Component.h"

#include "lib/vector.h"

#include <unordered_map>
#include <algorithm>
#include <atomic>
#include <string>
#include <optional>

#include <stdexcept>

namespace ag
{
	/*
	struct EntityInfo
	{
		EntityID ID{0};
	};
	*/

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
		EntityID SpawnEntity(Cs... component)
		{
			return InstantiateEntity(component...);
		}

		/**
		* Adds an entity to the spawn buffer from a vector of type-erased components
		*/
		EntityID SpawnEntity(vector<ag::Component>& components)
		{
			return InstantiateEntity(components);
		}

		/**
		 * Returns an EntityInfo struct from an index.
		 * @param i The index of the entity in the collection.
		 * @return A copy of the EntityInfo struct of the entity
		*/
		EntityID GetEntityInfo(size_t i) const
		{
			if (i >= entities.size() || i < 0)
			{
				Log::Error(sfmt("ArchetypeCollection GetEntityInfo index {} out of range ({})", i, entities.size()));
				return EntityID{};
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
			{
				Log::Error(sfmt("ArchetypeCollection GetComponent index {} out of range ({})", i, entities.size()));
				throw std::out_of_range("ArchetypeCollection GetComponent index out of range");
			}

			/// TODO: is this check too intensive to do a gazillion times a second? can we cache this some way idk
			/// TODO: Consider a SIMD compare. Since ComponentTypeID is 1byte, should be SUPER easy
			auto componentTypeIter = std::find(ComponentTypes.begin(), ComponentTypes.end(), ag::ComponentInfo::GetID<C>());
			if (componentTypeIter == ComponentTypes.end())
			{
				Log::Error(sfmt("Attempted to get component of type {}", std::string(typeid(C).name())));
				throw std::runtime_error("Attempted to get component of type " + std::string(typeid(C).name()));
			}

			auto componentTypeIndex = componentTypeIter - ComponentTypes.begin();
            
			// Return a reference to the component
			return (C&)(data[componentTypeIndex].at(i * sizeof(C)));
		}

		ArchetypeID GetID() const
		{
			return ID;
		}

		size_t GetEntityCount() const { return entities.size(); }

		/**
		 * Searches for the index of an entity in the collection by its ID.
		 * @param id The id of the entity to search for.
		 * @return The index of the found entity, or -1 if no entity was found.
		*/
		size_t GetIndexByID(EntityID id) const;

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

		/**
		 * Determines if this Archetype contains a component of the matching type. 
		*/
		bool HasComponent(ComponentTypeID ctype)
		{
			return std::find(ComponentTypes.begin(), ComponentTypes.end(), ctype) != ComponentTypes.end();
		}

		ArchetypeCollection(const ComponentSet& components);
		ArchetypeCollection(ArchetypeCollection& other) = delete;
		ArchetypeCollection& operator= (const ArchetypeCollection&) = delete;
		~ArchetypeCollection();

		ComponentSet GetComponentSet() const;

	private:

		template <typename... Cs>
		EntityID InstantiateEntity(Cs... component)
		{
			EntityID newEntityID = GetNextID();
			entitiesToSpawn.push_back(newEntityID);

			int i = 0;
			// We assume the components are in the order matching the archetype and add them to each component array (in the spawn buffer)
			(AddComponent((byte*)&component, i++, sizeof(component), spawnBuffer), ...);

			return newEntityID;
		}


		EntityID InstantiateEntity(vector<ag::Component>& components)
		{
			EntityID newEntityID = GetNextID();
			entitiesToSpawn.push_back(newEntityID);

			for (int i = 0; i < components.size(); i++)
			{
				AddComponent((byte*)components[i].GetRawData(), i, components[i].Size(), spawnBuffer);
			}

			return newEntityID;
		}

		const ArchetypeID ID;
		// The component types that make up this archetype
		ComponentSet ComponentTypes;
		// The entity data stored in this archetype. Each component array is a byte array of a specific component type.
		vector<ComponentArray> data;

		EntityID GetNextID();
		// The ID of the next entity to spawn
		EntityID NextEntityID;

		// Entity creation/destruction buffers
		vector<ComponentArray> spawnBuffer;
		vector<EntityID> entitiesToSpawn;
		void ResolveSpawnBuffer();
		vector<size_t> entitiesToDestroy;
		void ResolveDestroyBuffer();

		// Core entity data (a component that every entity has)
		vector<EntityID> entities;

		void AddComponent(byte* bytes, int i, int n, vector<ComponentArray>& target);

		static std::atomic<ArchetypeID> nextArchetypeID;
	};
}