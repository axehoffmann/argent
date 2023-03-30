#include <criterion/criterion.h>
#include "../src/core/Transform.h"

Test(transform, translation)
{
    ag::Transform t({0, 1, 0});

    glm::vec3 pos = t.GetPosition();
    cr_expect(pos == glm::vec3(0, 1, 0), "Transform error: Expected a position of (0, 1, 0), instead found (%d, %d, %d)", pos.x, pos.y, pos.z);

    t.Translate(glm::vec3(1.5f, 2, -1));
    pos = t.GetPosition();
    cr_expect(pos == glm::vec3(1.5f, 3, -1), "Translate error: Expected a position of (1, 3, -1), instead found (%d, %d, %d)", pos.x, pos.y, pos.z);
}

Test(transform, dirty_flag)
{
    ag::Transform t({0, 1, 0});
    cr_expect(!t.HasChanged(), "HasChanged flag error: Expected false, instead found true");
    t.Translate(glm::vec3(1.5f, 2, -1));
    cr_expect(t.HasChanged(), "HasChanged flag error: Expected true, instead found false");
    t.Clean();
    cr_expect(!t.HasChanged(), "HasChanged flag error: Expected false, instead found true");
}