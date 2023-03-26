#include "Entity.h"

ag::Entity::Entity(EntityID id)
{
    ID = id;
    archetype = ArchetypeCollection::GetArchetypeFromEntityID(ID);
}

EntityID ag::Entity::GetID()
{
    return ID;
}