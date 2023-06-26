#include "TestFramework.h"
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

void Setup()
{
    collection1 = new ag::ArchetypeCollection({ ag::ComponentInfo::GetID<ComponentA>() });
    collection2 = new ag::ArchetypeCollection({ ag::ComponentInfo::GetID<ComponentA>(), ag::ComponentInfo::GetID<ComponentB>() });
}

void Cleanup()
{
    delete collection1;
    delete collection2;
}

// Entity Creation and Destruction
void create_destroy()
{
    collection1->SpawnEntity("e1", ComponentA(100));

    ag_expect(collection1->GetEntityCount() == 0, "Entity creation should have been buffered");
    collection1->ResolveBuffers();
    ag_expect(collection1->GetEntityCount() == 1, "Expected 1 entity, instead found {}", collection1->GetEntityCount());

    for (int i = 0; i < 5; i++)
    {
        collection1->SpawnEntity("e2", ComponentA(i));
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
void data_access()
{
    EntityID lastEntity;
    for (int i = 0; i < 5; i++)
    {
        lastEntity = collection2->SpawnEntity("entity", ComponentA(100 + i), ComponentB(i));
    }

    ag_expect(collection2->GetComponent<ComponentA>(0) == nullptr, "Expected nullptr as entity should be in buffer");

    collection2->ResolveBuffers();

    ComponentA* componentA = collection2->GetComponent<ComponentA>(0);
    ag_expect(componentA != nullptr, "Expected pointer to ComponentA, instead found nullptr");
    ag_expect(componentA->value == 100, "Expected initial ComponentA.value of 100, instead found {}", componentA->value);
    componentA->value = 10;
    ag_expect(collection2->GetComponent<ComponentA>(0)->value == 10, "GetComponent error: Expected updated ComponentA.value of 10, instead found {}", collection2->GetComponent<ComponentA>(0)->value);

    ComponentB* componentB = collection2->GetComponent<ComponentB>(2);
    ag_expect(componentB != nullptr, "Expected pointer to ComponentB, instead found nullptr");
    ag_expect(componentB->partner == 2, "Expected initial ComponentB.partner of 2, instead found {}", componentB->partner);
    componentB->partner = lastEntity;
    ag_expect(collection2->GetComponent<ComponentB>(2)->partner == lastEntity, "Expected updated ComponentB.partner of {}, instead found {}", lastEntity, collection2->GetComponent<ComponentB>(2)->partner);

    size_t partnerIndex = collection2->GetIndexByID(componentB->partner);
    ag_expect(partnerIndex == 4, "Expected last entity at index 4, instead found {}", partnerIndex);
    ComponentA* partnerComponentA = collection2->GetComponent<ComponentA>(partnerIndex);
    ag_expect(partnerComponentA->value == 104, "Expected last entity to have ComponentA.value of 104, instead found {}", partnerComponentA->value);
}

void entity_class()
{
    EntityID lastEntity;
    for (int i = 0; i < 5; i++)
    {
        lastEntity = collection2->SpawnEntity("entity", ComponentA(100 + i), ComponentB(i));
    }
    collection2->ResolveBuffers();

    ag::Entity entityA(lastEntity);

    ag_expect(entityA.GetID() == lastEntity, "Expected entity ID to be {}, instead found {}", lastEntity, entityA.GetID());

    ag_expect(entityA.Get<ComponentA>()->value == 104, "Expected entity's A value to be 104, instead found {}", entityA.Get<ComponentA>()->value);
    ag_expect(entityA.Get<ComponentB>()->partner == 4, "Expected entity's B value to be 4, instead found {}", entityA.Get<ComponentB>()->partner);

    entityA.Get<ComponentA>()->value = -10;

    ag_expect(entityA.Get<ComponentA>()->value == -10, "Expected entity's A value to be -10, instead found {}", entityA.Get<ComponentA>()->value);
}

int main()
{
    Test::Name("Archetypes");
    Test::init = Setup;
    Test::clean = Cleanup;

    Test::Case("Create/Destroy", create_destroy);
    Test::Case("Data Access", data_access);
    Test::Case("Entity Class", entity_class);

    Test::Run();
}