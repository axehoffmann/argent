#include "../TestFramework.h"
#include "ecs/ECS.h"

#include "ecs/EntityRef.h"

namespace entityref_test
{
    ag::ArchetypeCollection* collection1;

    struct ComponentA
    {
        int value;
        ComponentA(int val)
        {
            value = val;
        }
    };

	$ITest(entity_ref, {});

    $Init(entity_ref)
    {
        collection1 = new ag::ArchetypeCollection({ ag::ComponentInfo::GetID<ComponentA>() });
    }

    $Cleanup(entity_ref)
    {
        delete collection1;
    }

    $Case(basic, entity_ref)
    {
        for (size_t i = 0; i < 10; i++)
        {
            collection1->SpawnEntity(ComponentA(i));
        }
        ag::EntityRef precious = ag::EntityRef(collection1->SpawnEntity(ComponentA(420)));
        for (size_t i = 0; i < 5; i++)
        {
            collection1->SpawnEntity(ComponentA(i + 10));
        }

        // Expect that no matter what we delete, EntityRef will always point to the correct entity
        // Once we delete precious, we should never enter a ag::EntityRef::Refresh() clause.
        std::array<int, 15> indices = { 0, 1, 2, 10, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        for (int index : indices)
        {
            collection1->DestroyEntity(index);
            collection1->ResolveBuffers();

            if (precious.Refresh())
            {
                ag_expect(precious.Get<ComponentA>().value == 420, "Expected ComponentA::value of 420, instead found {}", precious.Get<ComponentA>().value);
            }
        }
    }
}