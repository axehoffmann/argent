#pragma once

#include "ECSTypes.h"
#include "ArchetypeCollection.h"

namespace ag
{
	/**
	 * A class to wrap the concept of an entity into a shareable object.
	 * An entity is truly only a set of components in an ArchetypeCollection, this class simply indexes into that collection.
	*/
	class Entity
	{
	public:
		/**
		 * Creates an Entity indexer from an ID.
		 * @param id The ID of the entity to index to.
		*/
		Entity(EntityID id);

		/**
		 * Gets a component from this entity.
		 * @tparam C The type of component to get.
		 * @return A pointer to the component, or nullptr if the search was unsuccessful.
		*/
		template <typename C>
		C* Get()
		{
			size_t index = archetype->GetIndexByID(ID);
			if (index == -1)
				return nullptr;
			return archetype->GetComponent<C>(index);
		}

		EntityID GetID();

	private:
		EntityID ID;
		ArchetypeCollection* archetype;
	};
}