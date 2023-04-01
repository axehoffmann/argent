#include <criterion/criterion.h>
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

void Setup(void)
{
    cA = new ag::ArchetypeCollection({ ag::Component::GetID<ComponentA>() });
    cAB = new ag::ArchetypeCollection({ ag::Component::GetID<ComponentA>(), ag::Component::GetID<ComponentB>() });
    cAC = new ag::ArchetypeCollection({ ag::Component::GetID<ComponentA>(), ag::Component::GetID<ComponentC>() });

    world = new ag::World();

    world->AddArchetype(cA);
    world->AddArchetype(cAB);
}

void Cleanup(void)
{
    delete world;

    delete cA;
    delete cAB;
    delete cAC;
}

TestSuite(world, .init=Setup, .fini=Cleanup);

bool qEmpty = true;
std::vector<int> qIter;
Test(world, empty_query)
{
    qEmpty = true;
    world->Query(+[](ag::QueryResult<ComponentC> c) {
        qEmpty = false;
    });

    cr_expect(qEmpty, "Query error: expected empty query to not execute callback function");
}

int qCount = 0;
Test(world, success_empty_query)
{
    // Test valid query with empty archetypes
    qCount = 0;
    world->Query(+[](ag::QueryResult<ComponentA> a) {
        qCount = a.Length();
    });
    cr_expect(qCount == 0, "Query error: expected query to have length 0, instead found %d", qCount);
}

Test(world, one_populated)
{
    // Test valid query with 1 populated archetype
    for (size_t i = 0; i < 5; i++)
        cA->SpawnEntity("", ComponentA(i));
    cA->ResolveBuffers();

    qCount = 0;
    world->Query(+[](ag::QueryResult<ComponentA> a) {
        qCount = a.Length();
    });
    cr_expect(qCount == 5, "Query error: expected query to have length 5, instead found %d", qCount);
}

Test(world, two_populated)
{
    // Test valid query with 2 populated archetypes
    for (size_t i = 0; i < 5; i++)
        cA->SpawnEntity("", ComponentA(i));
    cA->ResolveBuffers();

    for (size_t i = 0; i < 5; i++)
        cAB->SpawnEntity("", ComponentA(i + 5), ComponentB());
    cAB->ResolveBuffers();

    std::vector<int> expected { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    qIter.clear();
    world->Query(+[](ag::QueryResult<ComponentA> a) {
        for (size_t i = 0; i < a.Length(); i++)
            qIter.push_back(a[i]->value);
    });
    cr_expect(qIter == expected, "Query error: expected query to iterate all 10 entities");

    // Test valid query on a set of Components
    expected = std::vector<int> { 5, 6, 7, 8, 9 };
    qIter.clear();
    world->Query(+[](ag::QueryResult<ComponentA> a, ag::QueryResult<ComponentB> b) {
        for (size_t i = 0; i < a.Length(); i++)
            qIter.push_back(a[i]->value);
    });
    cr_expect(qIter == expected, "Query error: expected query to iterate 5 AB entities");
}
