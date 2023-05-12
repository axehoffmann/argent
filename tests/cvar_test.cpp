#include "TestFramework.h"
#include "../src/core/cvars/CVARs.h"

#include <glm/glm.hpp>

#include <string>

void basic()
{
    ag::CVARs::Create<int>("test_int", " ", 2);
    ag::CVARs::Create<std::string>("test_string", " ", "test");

    int t_int = ag::CVARs::Get<int>("test_int");
    std::string t_string = ag::CVARs::Get<std::string>("test_string");

    ag_expect(t_int == 2, "Expected test_int to have initial value of 2, instead found {}", t_int);
    ag_expect(t_string == "test", "Expected test_string to have initial value of 'test', instead found {}", t_string);

    ag::CVARs::Set<int>("test_int", 5);
    // Testing failed set
    ag::CVARs::Set<int>("best_int", -5);

    ag_expect(t_int == 5, "Expected test_int to have new value of 5, instead found {}", t_int);
}

int main()
{
    Test::Name("CVARs");

    Test::Case("Basic", basic);

    Test::Run();
}