#include "../TestFramework.h"
#include "ecs/ECS.h"
#include "ecs/EntityRef.h"

#include <memory>

namespace archetype_test {

    std::unique_ptr<ag::ArchetypeCollection> collection1;
    std::unique_ptr<ag::ArchetypeCollection> collection2;

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

    $UTest(archetype);

    $Init(archetype)
    {
        collection1 = std::make_unique<ag::ArchetypeCollection>(ComponentSet{ ag::ComponentInfo::GetID<ComponentA>() });
        collection2 = std::make_unique<ag::ArchetypeCollection>(ComponentSet{ ag::ComponentInfo::GetID<ComponentA>(), ag::ComponentInfo::GetID<ComponentB>() });
    }

    $Cleanup(archetype)
    {
        // collection1.reset();
        // collection2.reset();
    }

    // Entity Creation and Destruction
    $Case(create_destroy, archetype)
    {
        collection1->SpawnEntity(ComponentA(100));

        ag_expect(collection1->GetEntityCount() == 0, "Entity creation should have been buffered");
        collection1->ResolveBuffers();
        ag_expect(collection1->GetEntityCount() == 1, "Expected 1 entity, instead found {}", collection1->GetEntityCount());

        for (int i = 0; i < 5; i++)
        {
            collection1->SpawnEntity(ComponentA(i));
        }

        ag_expect(collection1->GetEntityCount() == 1, "Expected 1 entity before resolving spawn buffer, instead found {}", collection1->GetEntityCount());
        collection1->ResolveBuffers();
        ag_expect(collection1->GetEntityCount() == 6, "Expected 6 entities after resolving spawn buffer, instead found {}", collection1->GetEntityCount());

        for (int i = 0; i < 6; i++)
        {
            collection1->DestroyEntity(0);
        }

        ag_expect(collection1->GetEntityCount() == 6, "Expected 6 entities before resolving destroy buffer, instead found {}", collection1->GetEntityCount());
        collection1->ResolveBuffers();
        ag_expect(collection1->GetEntityCount() == 0, "Expected 6 entities after resolving destroy buffer, instead found {}", collection1->GetEntityCount());
    }

    // Data access and modification
    $Case(data_access, archetype)
    {
        EntityID lastEntity;
        for (int i = 0; i < 5; i++)
        {
            lastEntity = collection2->SpawnEntity(ComponentA(100 + i), ComponentB(i));
        }

        bool err = false;
        try
        {
            collection2->GetComponent<ComponentA>(0);
        }
        catch (const std::exception&)
        {
            err = true;
        }
        ag_expect(err, "Expected an error as entity should be in buffer and innacessible");

        collection2->ResolveBuffers();

        ComponentA& componentA = collection2->GetComponent<ComponentA>(0);
        ag_expect(componentA.value == 100, "Expected initial ComponentA.value of 100, instead found {}", componentA.value);
        componentA.value = 10;
        ag_expect(collection2->GetComponent<ComponentA>(0).value == 10, "GetComponent error: Expected updated ComponentA.value of 10, instead found {}", collection2->GetComponent<ComponentA>(0).value);

        ComponentB& componentB = collection2->GetComponent<ComponentB>(2);
        ag_expect(componentB.partner == 2, "Expected initial ComponentB.partner of 2, instead found {}", componentB.partner);
        componentB.partner = lastEntity;
        ag_expect(collection2->GetComponent<ComponentB>(2).partner == lastEntity, "Expected updated ComponentB.partner of {}, instead found {}", lastEntity, collection2->GetComponent<ComponentB>(2).partner);

        size_t partnerIndex = collection2->GetIndexByID(componentB.partner);
        ag_expect(partnerIndex == 4, "Expected last entity at index 4, instead found {}", partnerIndex);
        ComponentA& partnerComponentA = collection2->GetComponent<ComponentA>(partnerIndex);
        ag_expect(partnerComponentA.value == 104, "Expected last entity to have ComponentA.value of 104, instead found {}", partnerComponentA.value);
    }

    $Case(entity_class, archetype)
    {
        EntityID lastEntity;
        for (int i = 0; i < 5; i++)
        {
            lastEntity = collection2->SpawnEntity(ComponentA(100 + i), ComponentB(i));
        }
        collection2->ResolveBuffers();

        ag::EntityRef entityA(lastEntity);

        bool operated = false;
        entityA.Operate([&](auto en) 
        {
            operated = true;
            // ag_expect(en.GetID() == lastEntity, "Expected entity ID to be {}, instead found {}", lastEntity, en.GetID());

            ag_expect(en.Get<ComponentA>().value == 104, "Expected entity's A value to be 104, instead found {}", en.Get<ComponentA>().value);
            ag_expect(en.Get<ComponentB>().partner == 4, "Expected entity's B value to be 4, instead found {}", en.Get<ComponentB>().partner);

            en.Get<ComponentA>().value = -10;

            ag_expect(en.Get<ComponentA>().value == -10, "Expected entity's A value to be -10, instead found {}", en.Get<ComponentA>().value);
        });

        ag_expect(operated, "Expected to operate on the valid entity");


    }
}