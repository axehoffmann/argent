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
    cA = new ag::ArchetypeCollection({ ag::Component::GetID<ComponentA>() });
    cAB = new ag::ArchetypeCollection({ ag::Component::GetID<ComponentA>(), ag::Component::GetID<ComponentB>() });
    cAC = new ag::ArchetypeCollection({ ag::Component::GetID<ComponentA>(), ag::Component::GetID<ComponentC>() });

    world = new ag::World();

    world->AddArchetype(cA);
    world->AddArchetype(cAB);
}

void Cleanup()
{
    delete world;

    delete cA;
    delete cAB;
    delete cAC;
}

void empty_query()
{
    bool qEmpty = true;
    std::function<void(ag::QueryResult<ComponentC>)> lm = [&qEmpty](ag::QueryResult<ComponentC> c) {
        qEmpty = false;
    };
    world->Query(lm);

    Test::Expect(qEmpty, "Expected empty query to not execute callback function");
}

void success_empty_query()
{
    // Test valid query with empty archetypes
    int qCount = 0;
    std::function<void(ag::QueryResult<ComponentA>)> lm = [&qCount](ag::QueryResult<ComponentA> a) {
        qCount = a.Length();
    };
    world->Query(lm);
    Test::Expect(qCount == 0, "Expected query to have length 0, instead found {}", qCount);
}

void one_populated()
{
    // Test valid query with 1 populated archetype
    for (size_t i = 0; i < 5; i++)
        cA->SpawnEntity("", ComponentA(i));
    cA->ResolveBuffers();

    int qCount = 0;
    std::function<void(ag::QueryResult<ComponentA>)> lm = [&qCount](ag::QueryResult<ComponentA> a) {
        qCount = a.Length();
    };
    world->Query(lm);
    Test::Expect(qCount == 5, "Expected query to have length 5, instead found {}", qCount);
}

void two_populated()
{
    // Test valid query with 2 populated archetypes
    for (size_t i = 0; i < 5; i++)
        cA->SpawnEntity("", ComponentA(i));
    cA->ResolveBuffers();

    for (size_t i = 0; i < 5; i++)
        cAB->SpawnEntity("", ComponentA(i + 5), ComponentB());
    cAB->ResolveBuffers();

    std::vector<int> expected { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::vector<int> qIter;

    qIter.clear();
    std::function<void(ag::QueryResult<ComponentA>)> lm = [&qIter](ag::QueryResult<ComponentA> a) {
        for (size_t i = 0; i < a.Length(); i++)
            qIter.push_back(a[i]->value);
    };
    world->Query(lm);
    Test::Expect(qIter == expected, "Expected query to iterate all 10 entities");

    // Test valid query on a set of Components
    expected = std::vector<int> { 5, 6, 7, 8, 9 };
    qIter.clear();
    std::function<void(ag::QueryResult<ComponentA>, ag::QueryResult<ComponentB>)> lm2 = [&qIter](ag::QueryResult<ComponentA> a, ag::QueryResult<ComponentB> b) {
        for (size_t i = 0; i < a.Length(); i++)
            qIter.push_back(a[i]->value);
    };
    world->Query(lm2);
    Test::Expect(qIter == expected, "Expected query to iterate 5 AB entities");
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