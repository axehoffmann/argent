#include "../TestFramework.h"
#include "ecs/ECS.h"

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
    ag_expect(ag::ComponentInfo::GetID<ComponentA>() == 1, "Expected first ID of 1, found {}", ag::ComponentInfo::GetID<ComponentA>());
    ag_expect(ag::ComponentInfo::GetID<ComponentB>() == 2, "Expected second ID of 2, found {}", ag::ComponentInfo::GetID<ComponentB>());
    ag_expect(ag::ComponentInfo::GetID<ComponentA>() == 1, "Expected first ID to remember 1, found {}", ag::ComponentInfo::GetID<ComponentA>());
}

void component_size()
{
    // Getting size by component ID (important for strides in archetype storage)
    ComponentTypeID compA = ag::ComponentInfo::GetID<ComponentA>();
    ag_expect(ag::ComponentInfo::GetSize(compA) == sizeof(ComponentA), "Expected size of {}, found {}", sizeof(ComponentA), ag::ComponentInfo::GetSize(compA));
    ComponentTypeID compB = ag::ComponentInfo::GetID<ComponentB>();
    ag_expect(ag::ComponentInfo::GetSize(compB) == sizeof(ComponentB), "Expected size of {}, found {}", sizeof(ComponentB), ag::ComponentInfo::GetSize(compB));
}

int main()
{
    Test::Name("Components");

    Test::Case("ID Allocation", id_allocation);
    Test::Case("Component Size", component_size);

    Test::Run();
}