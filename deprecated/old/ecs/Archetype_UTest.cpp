#include "../TestFramework.h"
#include "ecs/ECS.h"
#include "ecs/EntityRef.h"
#include "tests/ecs/ECS_Mock.h"

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
        collection1 = std::make_unique<ag::ArchetypeCollection>(ComponentSet{ ag::ComponentInfo::GetID<mock::CmptA>() });
        collection2 = std::make_unique<ag::ArchetypeCollection>(ComponentSet{ ag::ComponentInfo::GetID<mock::CmptA>(), ag::ComponentInfo::GetID<mock::CmptB>() });
    }

    $Cleanup(archetype)
    {
        // collection1.reset();
        // collection2.reset();
    }

    // Entity Creation and Destruction
    $Case(create_destroy, archetype)
    {
        collection1->SpawnEntity(mock::CmptA(100));

        ag_expect(collection1->GetEntityCount() == 0, "Entity creation should have been buffered");
        collection1->ResolveBuffers();
        ag_expect(collection1->GetEntityCount() == 1, "Expected 1 entity, instead found {}", collection1->GetEntityCount());

        for (int i = 0; i < 5; i++)
        {
            collection1->SpawnEntity(mock::CmptA(i));
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
            lastEntity = collection2->SpawnEntity(mock::CmptA(100 + i), mock::CmptB(i));
        }

        bool err = false;
        try
        {
            collection2->GetComponent<mock::CmptA>(0);
        }
        catch (const std::exception&)
        {
            err = true;
        }
        ag_expect(err, "Expected an error as entity should be in buffer and innacessible");

        collection2->ResolveBuffers();

        mock::CmptA& componentA = collection2->GetComponent<mock::CmptA>(0);
        ag_expect(componentA.value == 100, "Expected initial ComponentA.value of 100, instead found {}", componentA.value);
        componentA.value = 10;
        ag_expect(collection2->GetComponent<mock::CmptA>(0).value == 10, "GetComponent error: Expected updated ComponentA.value of 10, instead found {}", collection2->GetComponent<mock::CmptA>(0).value);

        mock::CmptB& componentB = collection2->GetComponent<mock::CmptB>(2);
        ag_expect(componentB.partner == 2, "Expected initial ComponentB.partner of 2, instead found {}", componentB.partner);
        componentB.partner = lastEntity;
        ag_expect(collection2->GetComponent<mock::CmptB>(2).partner == lastEntity, "Expected updated ComponentB.partner of {}, instead found {}", lastEntity, collection2->GetComponent<mock::CmptB>(2).partner);

        size_t partnerIndex = collection2->GetIndexByID(componentB.partner);
        ag_expect(partnerIndex == 4, "Expected last entity at index 4, instead found {}", partnerIndex);
        mock::CmptA& partnerComponentA = collection2->GetComponent<mock::CmptA>(partnerIndex);
        ag_expect(partnerComponentA.value == 104, "Expected last entity to have ComponentA.value of 104, instead found {}", partnerComponentA.value);
    }
}