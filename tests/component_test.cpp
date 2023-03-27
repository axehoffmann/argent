#include <criterion/criterion.h>
#include "../src/ecs/ECS.h"

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

Test(component, id_allocation)
{
    cr_expect(ag::Component::GetID<ComponentA>() == 1, "GetID error: Expected first ID of 1, found %d", ag::Component::GetID<ComponentA>());
    cr_expect(ag::Component::GetID<ComponentB>() == 2, "GetID error: Expected second ID of 2, found %d", ag::Component::GetID<ComponentB>());
    cr_expect(ag::Component::GetID<ComponentA>() == 1, "GetID error: Expected first ID to remember 1, found %d", ag::Component::GetID<ComponentA>());
}

Test(component, componet_size)
{
    // Getting size by component ID (important for strides in archetype storage)
    ComponentTypeID compA = ag::Component::GetID<ComponentA>();
    cr_expect(ag::Component::GetSize(compA) == sizeof(ComponentA), "GetSize error: Expected size of %d, found %d", sizeof(ComponentA), ag::Component::GetSize(compA));
    ComponentTypeID compB = ag::Component::GetID<ComponentB>();
    cr_expect(ag::Component::GetSize(compB) == sizeof(ComponentB), "GetSize error: Expected size of %d, found %d", sizeof(ComponentB), ag::Component::GetSize(compB));
}