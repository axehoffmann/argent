#pragma once

#include "ECS.h"
#include "ArchetypeCollection.h"
#include "Entity.h"

namespace ag
{
	/// TODO: Do I want to template this on expected component types? Compile time cost = icky, but much safer code
	class EntityRef
	{
	public:
		EntityRef(EntityID entityID) : 
			id(entityID), 
			archetype(ag::ArchetypeCollection::GetArchetypeFromEntityID(entityID)),
			index(-1) {}

        /**
        * Returns whether this entity still exists
        */
        bool Exists()
        {
            if (archetype->GetEntityInfo(index) == id)
                return true;

            index = archetype->GetIndexByID(id);
            return index >= 0 && index < archetype->GetEntityCount();
        }

        template <typename functor>
        bool Operate(functor f)
        {
            if (!Exists())
                return false;

            f(Entity(archetype, index));
            return true;
        }

        /**
        * Mark the entity to be destroyed next update.
        */
        void Destroy()
        {
            if (Exists())
                archetype->DestroyEntity(index);
        }

        EntityID GetID()
        {
            return id;
        }

	private:
		EntityID id;
		ag::ArchetypeCollection* archetype;
		size_t index;
	};
}