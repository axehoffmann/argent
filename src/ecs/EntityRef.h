#pragma once

#include "ECS.h"
#include "ArchetypeCollection.h"

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

		bool Refresh()
		{
            if (archetype->GetEntityInfo(index).ID == id)
                return true;

			index = archetype->GetIndexByID(id);
			return index >= 0;
		}

        /**
        * Get a reference to a component from this entity.
        * @tparam The type of component to get.
        * @return A reference to the selected component
        */
        template <typename C>
        C& Get()
        {
            return archetype->GetComponent<C>(index);
        }

        /**
        * Get a copy of the info struct of the entity.
        */
        EntityInfo Info() const
        {
            return archetype->GetEntityInfo(index);
        }

        /**
        * Mark the entity to be destroyed next update.
        */
        void Destroy()
        {
            archetype->DestroyEntity(index);
        }

	private:
		EntityID id;
		std::shared_ptr<ag::ArchetypeCollection> archetype;
		size_t index;
	};
}