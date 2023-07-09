#include "TestFramework.h"
#include "../src/ecs/ECS.h"

#include <vector>

ag::ArchetypeCollection* cA;
ag::ArchetypeCollection* cAB;
ag::ArchetypeCollection* cAC;

ag::World* world;

struct ComponentA 
{
    int value;
    ComponentA(int val)
    {
        value = val;
    }
};

struct ComponentB { };

struct ComponentC { };

void Setup()
{
    cA = new ag::ArchetypeCollection({ ag::ComponentInfo::GetID<ComponentA>() });
    cAB = new ag::ArchetypeCollection({ ag::ComponentInfo::GetID<ComponentA>(), ag::ComponentInfo::GetID<ComponentB>() });
    cAC = new ag::ArchetypeCollection({ ag::ComponentInfo::GetID<ComponentA>(), ag::ComponentInfo::GetID<ComponentC>() });

    world = new ag::World();

    world->AddArchetype(cA);
    world->AddArchetype(cAB);
}

void Cleanup()
{
    delete world;
}

void empty_query()
{
    // Query has no matches (cAC not added to world)
    auto query = ag::Query<ComponentC>(world);

    ag_expect(query.Size() == 0, "Expected query to yield no results");
}

void success_empty_query()
{
    // Query has 2 matches but neither has any entities
    auto query = ag::Query<ComponentA>(world);

    ag_expect(query.Size() == 0, "Expected query to have length 0, instead found {}", query.Size());
}

void one_populated()
{
    // Test valid query with 1 populated archetype
    for (int i = 0; i < 5; i++)
        cA->SpawnEntity("", ComponentA(i));
    cA->ResolveBuffers();

    auto query = ag::Query<ComponentA>(world);

    ag_expect(query.Size() == 5, "Expected query to have length 5, instead found {}", query.Size());
}

void two_populated()
{
    // Test valid query with 2 populated archetypes
    for (int i = 0; i < 5; i++)
        cA->SpawnEntity("", ComponentA(i));
    cA->ResolveBuffers();

    for (int i = 0; i < 5; i++)
        cAB->SpawnEntity("", ComponentA(i + 5), ComponentB());
    cAB->ResolveBuffers();

    std::vector<int> expected { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::vector<int> qIter;

    auto queryA = ag::Query<ComponentA>(world);

    qIter.clear();
    for (auto entity : queryA)
    {
        qIter.push_back(entity.Get<ComponentA>().value);
    }
    ag_expect(qIter == expected, "Expected query to iterate all 10 entities, instead found {}", qIter.size());

    // Test valid query on a set of Components
    expected = std::vector<int> { 5, 6, 7, 8, 9 };

    auto queryAB = ag::Query<ComponentA, ComponentB>(world);

    qIter.clear();
    for (auto entity : queryAB)
    {
        qIter.push_back(entity.Get<ComponentA>().value);
    }
    ag_expect(qIter == expected, "Expected query to iterate 5 AB entities, instead found {}", qIter.size());
}


int main()
{
    Test::Name("Worlds");
    Test::init = Setup;
    Test::clean = Cleanup;
   
    Test::Case("Empty Query", empty_query);
    Test::Case("Empty Query 2", success_empty_query);
    Test::Case("One Populated", one_populated);
    Test::Case("Two Populated", two_populated);


    Test::Run();
}