#include "../TestFramework.h"
#include "ecs/ECS.h"

$UTest(components);

struct ComponentA 
{
    int value;
    ComponentA(int val)
    {
        value = val;
    }
};

struct ComponentB
{
    EntityID partner;
    ComponentB(EntityID p)
    {
        partner = p;
    }
};

$Case(id_allocation, components)
{
    ag_expect(ag::ComponentInfo::GetID<ComponentA>() == 1, "Expected first ID of 1, found {}", ag::ComponentInfo::GetID<ComponentA>());
    ag_expect(ag::ComponentInfo::GetID<ComponentB>() == 2, "Expected second ID of 2, found {}", ag::ComponentInfo::GetID<ComponentB>());
    ag_expect(ag::ComponentInfo::GetID<ComponentA>() == 1, "Expected first ID to remember 1, found {}", ag::ComponentInfo::GetID<ComponentA>());
}

$Case(component_size, components)
{
    // Getting size by component ID (important for strides in archetype storage)
    ComponentTypeID compA = ag::ComponentInfo::GetID<ComponentA>();
    ag_expect(ag::ComponentInfo::GetSize(compA) == sizeof(ComponentA), "Expected size of {}, found {}", sizeof(ComponentA), ag::ComponentInfo::GetSize(compA));
    ComponentTypeID compB = ag::ComponentInfo::GetID<ComponentB>();
    ag_expect(ag::ComponentInfo::GetSize(compB) == sizeof(ComponentB), "Expected size of {}, found {}", sizeof(ComponentB), ag::ComponentInfo::GetSize(compB));
}