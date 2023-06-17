#include "TestFramework.h"
#include "../src/ecs/ECS.h"
#include "../src/ecs/System.h"

#include <vector>


struct ComponentA
{
    int value;
    ComponentA(int val)
    {
        value = val;
    }
};

struct ComponentB { };

class TestSystem : public ag::System
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
private:
    static ag::System::SystemRegister<TestSystem> reg;
};

ag::System::SystemRegister<TestSystem> TestSystem::reg = ag::System::SystemRegister<TestSystem>();


void initialisation()
{
    size_t systemCount = ag::System::Systems().size();
    ag_expect(systemCount == 1, "Expected 1 registered system, instead found {}", systemCount);

    std::unique_ptr<ag::System> sys = ag::System::Systems().at(0)();
    TestSystem* pt = static_cast<TestSystem*>(sys.get());

    ag_expect(pt->testVal == 10, "Expected initial testVal of 10, instead found {}", pt->testVal);
}

void updating()
{
    std::unique_ptr<ag::System> sys = ag::System::Systems().at(0)();
    TestSystem* pt = static_cast<TestSystem*>(sys.get());

    for (size_t i = 0; i < 10; i++)
    {
        ag_expect(pt->testVal == 10 + i, "Expected total time of {}, instead found {}", 10 + i, pt->testVal);
        pt->Update(0.0);
    }
}

int main()
{
    Test::Name("Systems");

    Test::Case("Initialisation", initialisation);
    Test::Case("Updating", updating);


    Test::Run();
}