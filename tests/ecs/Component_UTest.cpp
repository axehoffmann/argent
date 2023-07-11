#include "../TestFramework.h"
#include "ecs/ECS.h"

namespace component_test
{
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
        ComponentTypeID id = ag::ComponentInfo::GetID<ComponentA>();
        ag_expect(ag::ComponentInfo::GetID<ComponentB>() == id + 1, "Expected second ID of {}, found {}", id + 1, ag::ComponentInfo::GetID<ComponentB>());
        ag_expect(ag::ComponentInfo::GetID<ComponentA>() == id, "Expected first ID to remember {}, found {}", id, ag::ComponentInfo::GetID<ComponentA>());
    }

    $Case(component_size, components)
    {
        // Getting size by component ID (important for strides in archetype storage)
        ComponentTypeID compA = ag::ComponentInfo::GetID<ComponentA>();
        ag_expect(ag::ComponentInfo::GetSize(compA) == sizeof(ComponentA), "Expected size of {}, found {}", sizeof(ComponentA), ag::ComponentInfo::GetSize(compA));
        ComponentTypeID compB = ag::ComponentInfo::GetID<ComponentB>();
        ag_expect(ag::ComponentInfo::GetSize(compB) == sizeof(ComponentB), "Expected size of {}, found {}", sizeof(ComponentB), ag::ComponentInfo::GetSize(compB));
    }
}