#pragma once

#include "ArchetypeCollection.h"

namespace ag
{
    struct IEntity
    {
        /**
        * Get a copy of the info struct of the entity.
        */
        EntityID Info() const
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

        template <typename C>
        bool HasComponent()
        {
            return archetype->HasComponent(ag::ComponentInfo::GetID<C>());
        }

        // This should not be copyable and cannot persist outside of an operation/transaction
        // because after entities are added or deleted this may no longer point to
        // the same entity.
        IEntity(const IEntity&) = delete;
        IEntity& operator=(const IEntity&) = delete;

        IEntity(ag::ArchetypeCollection* a, size_t i) : archetype(a), index(i) {}

    protected:
        ag::ArchetypeCollection* archetype;
        size_t index;
    };

   /**
    * An indexer object used to conveniently access the properties of entities in a query.
    */
    template <typename... Cs>
    struct Entity : IEntity
    {
        /**
        * Get a reference to a component from this entity.
        * @tparam The type of component to get. Must be a type in the query.
        * @return A reference to the selected component
        */
        template <typename C>
        C& Get()
        {
            // Component we are getting must be one of the types from the Query this entity is from
            static_assert((std::is_same_v<C, Cs> || ...));
            return archetype->GetComponent<C>(index);
        }

        Entity(ag::ArchetypeCollection* a, size_t i) : IEntity(a, i) {}
    };
}