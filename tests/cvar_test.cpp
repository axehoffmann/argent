#include "TestFramework.h"
#include "../src/core/cvars/CVARs.h"

#include <glm/glm.hpp>

#include <string>

std::string configPath = "assets/cvars.json";

void basic()
{
    ag::CVARs::Create<int>("test_int", " ", 2);
    ag::CVARs::Create<std::string>("test_string", " ", "test");

    int t_int = ag::CVARs::Get<int>("test_int");
    std::string t_string = ag::CVARs::Get<std::string>("test_string");

    ag_expect(t_int == 2, "Expected test_int to have initial value of 2, instead found {}", t_int);
    ag_expect(t_string == "test", "Expected test_string to have initial value of 'test', instead found {}", t_string);

    ag::CVARs::Set<int>("test_int", 5);
    ag::CVARs::Set<std::string>("test_string", "hello");
    // Testing failed set
    ag::CVARs::Set<int>("best_int", -5);
    ag::CVARs::Set<std::string>("best_string", "bye");

    t_int = ag::CVARs::Get<int>("test_int");
    t_string = ag::CVARs::Get<std::string>("test_string");

    ag_expect(t_int == 5, "Expected test_int to have new value of 5, instead found {}", t_int);
    ag_expect(t_string == "hello", "Expected test_string to have new value of 'hello', instead found {}", t_string);

    ag::CVARs::Reset("test_int");
    ag::CVARs::Reset("test_string");

    t_int = ag::CVARs::Get<int>("test_int");
    t_string = ag::CVARs::Get<std::string>("test_string");

    ag_expect(t_int == 2, "Expected test_int to have reset value of 2, instead found {}", t_int);
    ag_expect(t_string == "test", "Expected test_string to have reset value of 'test', instead found {}", t_string);
}

void load_file()
{
    ag::CVARs::LoadFile(configPath);
    int c_int = ag::CVARs::Get<int>("config_int");
    float c_float = ag::CVARs::Get<float>("config_float");

    ag_expect(c_int == 1, "Expected config_int to have initial value of 1, instead found {}", c_int);
    ag_expect(c_float == 0.5f, "Expected config_float to have initial value of 0.5f, instead found {}", c_float);

}

int main()
{
    Test::Name("CVARs");

    Test::Case("Basic", basic);
    Test::Case("Load from File", load_file);

    Test::Run();
}