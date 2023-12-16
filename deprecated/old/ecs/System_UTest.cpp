#include "../TestFramework.h"
#include "ecs/ECS.h"
#include "ecs/System.h"

#include <vector>

$UTest(systems);

struct ComponentA
{
    int value;
    ComponentA(int val)
    {
        value = val;
    }
};

struct ComponentB { };

class TestSystem : public ag::System<TestSystem>
{
public:
    int testVal{ 10 };

    void Init() override
    {

    }
    void Update(double dt) override
    {
        testVal += 1;
    }
    void FrameUpdate(double dt) override {}
};


$Case(initialisation, systems)
{
    size_t systemCount = ag::ISystem::Systems().size();
    ag_expect(systemCount == 1, "Expected 1 registered system, instead found {}", systemCount);

    std::unique_ptr<ag::ISystem> sys = ag::ISystem::Systems().at(0)();
    TestSystem* pt = static_cast<TestSystem*>(sys.get());

    ag_expect(pt->testVal == 10, "Expected initial testVal of 10, instead found {}", pt->testVal);
}

$Case(updating, systems)
{
    std::unique_ptr<ag::ISystem> sys = ag::ISystem::Systems().at(0)();
    TestSystem* pt = static_cast<TestSystem*>(sys.get());

    for (size_t i = 0; i < 10; i++)
    {
        ag_expect(pt->testVal == 10 + i, "Expected total time of {}, instead found {}", 10 + i, pt->testVal);
        pt->Update(0.0);
    }
}