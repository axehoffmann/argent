#include "../TestFramework.h"
#include "ecs/ECS.h"

#include "ecs/EntityRef.h"

namespace entityref_test
{
    std::shared_ptr<ag::World> world;
    ag::ArchetypeCollection* collection1;

    struct ComponentA
    {
        int value;
        ComponentA(int val)
        {
            value = val;
        }
    };

    struct Corona {};

	$ITest(entity_ref, {});

    $Init(entity_ref)
    {
        world = std::make_shared<ag::World>();
        collection1 = new ag::ArchetypeCollection(ComponentSet{ ag::ComponentInfo::GetID<ComponentA>() });
        world->AddArchetype(collection1);
    }

    $Cleanup(entity_ref)
    {
        world.reset();
    }

    $Case(basic, entity_ref)
    {
        for (size_t i = 0; i < 10; i++)
        {
            collection1->SpawnEntity(ComponentA(i));
        }
        ag::EntityRef precious = ag::EntityRef(collection1->SpawnEntity(ComponentA(420)), world.get());
        for (size_t i = 0; i < 5; i++)
        {
            collection1->SpawnEntity(ComponentA(i + 10));
        }

        // Expect that no matter what we delete, EntityRef will always point to the correct entity
        // Once we delete precious, we should never enter a ag::EntityRef::Operate() clause.
        std::array<int, 15> indices = { 0, 1, 2, 10, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        for (int index : indices)
        {
            collection1->DestroyEntity(index);
            collection1->ResolveBuffers();

            precious.Operate<ComponentA>([&](auto en)
            {
                ag_expect(en.HasComponent<ComponentA>(), "Expected entity to have ComponentA");
                ag_expect(en.HasComponent<Corona>(), "Expected entity to not have Corona component");

                ag_expect(en.Get<ComponentA>().value == 420, "Expected ComponentA::value of 420, instead found {}", en.Get<ComponentA>().value);
            });
        }
    }
}