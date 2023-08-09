#include "../TestFramework.h"
#include "ecs/ECS.h"

#include <vector>

namespace world_test
{
    ag::ArchetypeCollection* cA;
    ag::ArchetypeCollection* cAB;
    ag::ArchetypeCollection* cAC;

    ag::World* wrld;

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

    $ITest(world, {});

    $Init(world)
    {
        cA = new ag::ArchetypeCollection({ ag::ComponentInfo::GetID<ComponentA>() });
        cAB = new ag::ArchetypeCollection({ ag::ComponentInfo::GetID<ComponentA>(), ag::ComponentInfo::GetID<ComponentB>() });
        cAC = new ag::ArchetypeCollection({ ag::ComponentInfo::GetID<ComponentA>(), ag::ComponentInfo::GetID<ComponentC>() });

        wrld = new ag::World();

        wrld->AddArchetype(cA);
        wrld->AddArchetype(cAB);
    }

    $Cleanup(world)
    {
        delete wrld;
    }


    $Case(empty_query, world)
    {
        // Query has no matches (cAC not added to world)
        auto query = ag::Query<ComponentC>(wrld);

        ag_expect(query.Size() == 0, "Expected query to yield no results");
    }

    $Case(success_empty_query, world)
    {
        // Query has 2 matches but neither has any entities
        auto query = ag::Query<ComponentA>(wrld);

        ag_expect(query.Size() == 0, "Expected query to have length 0, instead found {}", query.Size());
    }

    $Case(one_populated, world)
    {
        // Test valid query with 1 populated archetype
        for (int i = 0; i < 5; i++)
            cA->SpawnEntity(ComponentA(i));
        cA->ResolveBuffers();

        auto query = ag::Query<ComponentA>(wrld);

        ag_expect(query.Size() == 5, "Expected query to have length 5, instead found {}", query.Size());
    }

    $Case(two_populated, world)
    {
        // Test valid query with 2 populated archetypes
        for (int i = 0; i < 5; i++)
            cA->SpawnEntity(ComponentA(i));
        cA->ResolveBuffers();

        for (int i = 0; i < 5; i++)
            cAB->SpawnEntity(ComponentA(i + 5), ComponentB());
        cAB->ResolveBuffers();

        std::vector<int> expected { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        std::vector<int> qIter;

        auto queryA = ag::Query<ComponentA>(wrld);

        qIter.clear();
        for (auto entity : queryA)
        {
            qIter.push_back(entity.Get<ComponentA>().value);
        }
        ag_expect(qIter == expected, "Expected query to iterate all 10 entities, instead found {}", qIter.size());

        // Test valid query on a set of Components
        expected = std::vector<int>{ 5, 6, 7, 8, 9 };

        auto queryAB = ag::Query<ComponentA, ComponentB>(wrld);

        qIter.clear();
        for (auto entity : queryAB)
        {
            qIter.push_back(entity.Get<ComponentA>().value);
        }
        ag_expect(qIter == expected, "Expected query to iterate 5 AB entities, instead found {}", qIter.size());
    }
}