#include "TestFramework.h"
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

void id_allocation()
{
    ag_expect(ag::Component::GetID<ComponentA>() == 1, "Expected first ID of 1, found {}", ag::Component::GetID<ComponentA>());
    ag_expect(ag::Component::GetID<ComponentB>() == 2, "Expected second ID of 2, found {}", ag::Component::GetID<ComponentB>());
    ag_expect(ag::Component::GetID<ComponentA>() == 1, "Expected first ID to remember 1, found {}", ag::Component::GetID<ComponentA>());
}

void component_size()
{
    // Getting size by component ID (important for strides in archetype storage)
    ComponentTypeID compA = ag::Component::GetID<ComponentA>();
    ag_expect(ag::Component::GetSize(compA) == sizeof(ComponentA), "Expected size of {}, found {}", sizeof(ComponentA), ag::Component::GetSize(compA));
    ComponentTypeID compB = ag::Component::GetID<ComponentB>();
    ag_expect(ag::Component::GetSize(compB) == sizeof(ComponentB), "Expected size of {}, found {}", sizeof(ComponentB), ag::Component::GetSize(compB));
}

int main()
{
    Test::Name("Components");

    Test::Case("ID Allocation", id_allocation);
    Test::Case("Component Size", component_size);

    Test::Run();
}