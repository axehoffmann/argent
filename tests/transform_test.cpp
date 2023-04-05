#include "TestFramework.h"
#include "../src/core/Transform.h"

void translation()
{
    ag::Transform t({0, 1, 0});

    glm::vec3 pos = t.GetPosition();
    ag_expect(pos == glm::vec3(0, 1, 0), "Expected a position of (0, 1, 0), instead found ({}, {}, {})", pos.x, pos.y, pos.z);

    t.Translate(glm::vec3(1.5f, 2, -1));
    pos = t.GetPosition();
    ag_expect(pos == glm::vec3(1.5f, 3, -1), "Expected a position of (1, 3, -1), instead found ({}, {}, {})", pos.x, pos.y, pos.z);
}

void dirty_flag()
{
    ag::Transform t({0, 1, 0});
    ag_expect(!t.HasChanged(), "Expected false, instead found true");
    t.Translate(glm::vec3(1.5f, 2, -1));
    ag_expect(t.HasChanged(), "Expected true, instead found false");
    t.Clean();
    ag_expect(!t.HasChanged(), "Expected false, instead found true");
}


int main()
{
    Test::Name("Transforms");

    Test::Case("Translation", translation);
    Test::Case("Dirty Flag", dirty_flag);

    Test::Run();
}