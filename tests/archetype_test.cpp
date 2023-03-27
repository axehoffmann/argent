#include <criterion/criterion.h>
#include "../src/ecs/ECS.h"

ag::ArchetypeCollection* collection1;
ag::ArchetypeCollection* collection2;

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

void Setup(void)
{
    collection1 = new ag::ArchetypeCollection({ ag::Component::GetID<ComponentA>() });
    collection2 = new ag::ArchetypeCollection({ ag::Component::GetID<ComponentA>(), ag::Component::GetID<ComponentB>() });
}

void Cleanup(void)
{
    delete collection1;
    delete collection2;
}

TestSuite(archetype, .init=Setup, .fini=Cleanup);

// Entity Creation and Destruction
/// TODO: Test entity parenting
Test(archetype, create_destroy)
{
    collection1->SpawnEntity("e1", ComponentA(100));

    cr_expect(collection1->GetEntityCount() == 0, "SpawnEntity error: Entity creation should have been buffered");
    collection1->ResolveBuffers();
    cr_expect(collection1->GetEntityCount() == 1, "ResolveBuffers error: Expected 1 entity, instead found %d", collection1->GetEntityCount());

    for (int i = 0; i < 5; i++)
    {
        collection1->SpawnEntity("e2", ComponentA(i));
    }

    cr_expect(collection1->GetEntityCount() == 1, "SpawnEntityError: Expected 1 entity before resolving spawn buffer, instead found %d", collection1->GetEntityCount());
    collection1->ResolveBuffers();
    cr_expect(collection1->GetEntityCount() == 6, "ResolveBuffers error: Expected 6 entities after resolving spawn buffer, instead found %d", collection1->GetEntityCount());

    for (int i = 0; i < 6; i++)
    {
        collection1->DestroyEntity(0);
    }

    cr_expect(collection1->GetEntityCount() == 6, "DestroyEntity error: Expected 6 entities before resolving destroy buffer, instead found %d", collection1->GetEntityCount());
    collection1->ResolveBuffers();
    cr_expect(collection1->GetEntityCount() == 0, "ResolveBuffers error: Expected 6 entities after resolving destroy buffer, instead found %d", collection1->GetEntityCount());
}

// Data access and modification
Test(archetype, data_access)
{
    EntityID lastEntity;
    for (int i = 0; i < 5; i++)
    {
        lastEntity = collection2->SpawnEntity("entity", ComponentA(100 + i), ComponentB(i));
    }

    cr_expect(collection2->GetComponent<ComponentA>(0) == nullptr, "GetComponent error: Expected nullptr as entity should be in buffer");

    collection2->ResolveBuffers();

    ComponentA* componentA = collection2->GetComponent<ComponentA>(0);
    cr_assert(componentA != nullptr, "GetComponent error: Expected pointer to ComponentA, instead found nullptr");
    cr_expect(componentA->value == 100, "GetComponent error: Expected initial ComponentA.value of 100, instead found %d", componentA->value);
    componentA->value = 10;
    cr_expect(collection2->GetComponent<ComponentA>(0)->value == 10, "GetComponent error: Expected updated ComponentA.value of 10, instead found %d", collection2->GetComponent<ComponentA>(0)->value);

    ComponentB* componentB = collection2->GetComponent<ComponentB>(2);
    cr_assert(componentB != nullptr, "GetComponent error: Expected pointer to ComponentB, instead found nullptr");
    cr_expect(componentB->partner == 2, "GetComponent error: Expected initial ComponentB.partner of 2, instead found %d", componentB->partner);
    componentB->partner = lastEntity;
    cr_expect(collection2->GetComponent<ComponentB>(2)->partner == lastEntity, "GetComponent error: Expected updated ComponentB.partner of %d, instead found %d", lastEntity, collection2->GetComponent<ComponentB>(2)->partner);

    size_t partnerIndex = collection2->GetIndexByID(componentB->partner);
    cr_expect(partnerIndex == 4, "GetIndexByID error: Expected last entity at index 4, instead found %d", partnerIndex);
    ComponentA* partnerComponentA = collection2->GetComponent<ComponentA>(partnerIndex);
    cr_expect(partnerComponentA->value == 104, "GetComponent error: Expected last entity to have ComponentA.value of 104, instead found %d", partnerComponentA->value);
}