#include "../TestFramework.h"
#include "core/Transform.h"

#include <fstream>
#include <nlohmann/json.hpp>

$UTest(transform);

const std::string jsonPath = "assets/entity.json";

$Case(data_access, transform)
{
    ag::Transform t({ 0, 1, 0 });
    glm::vec3 pos = t.GetPosition();
    ag_expect(pos == glm::vec3(0, 1, 0), "Expected a position of (0, 1, 0), instead found ({}, {}, {})", pos.x, pos.y, pos.z);

    t.SetPosition({ 1000, 10, -2000 });
    pos = t.GetPosition();
    ag_expect(pos == glm::vec3(1000, 10, -2000), "Expected a position of (1000, 10, -2000), instead found ({}, {}, {})", pos.x, pos.y, pos.z);

    glm::vec3 rot = t.GetEulerRotation();
    ag_expect(rot == glm::vec3(0, 0, 0), "Expected a rotation of (0, 0, 0), instead found ({}, {}, {})", rot.x, rot.y, rot.z);

    t.SetRotation({ glm::half_pi<float>(), 0, 0});
    rot = t.GetEulerRotation();
    ag_expect(rot == glm::vec3(glm::half_pi<float>(), 0, 0), "Expected a rotation of (0, 0, 0), instead found ({}, {}, {})", rot.x, rot.y, rot.z);
}

$Case(translation, transform)
{
    ag::Transform t({0, 1, 0});

    t.Translate(glm::vec3(1.5f, 2, -1));
    glm::vec3 pos = t.GetPosition();
    ag_expect(pos == glm::vec3(1.5f, 3, -1), "Expected a position of (1.5, 3, -1), instead found ({}, {}, {})", pos.x, pos.y, pos.z);

    t.Translate(glm::vec3(-0.5f, 197, -1));
    pos = t.GetPosition();
    ag_expect(pos == glm::vec3(1, 200, -2), "Expected a position of (1, 200, -2), instead found ({}, {}, {})", pos.x, pos.y, pos.z);
}

$Case(dirty_flag, transform)
{
    ag::Transform t({0, 1, 0});
    ag_expect(!t.HasChanged(), "Expected false, instead found true");
    t.Translate(glm::vec3(1.5f, 2, -1));
    ag_expect(t.HasChanged(), "Expected true, instead found false");
    t.Clean();
    ag_expect(!t.HasChanged(), "Expected false, instead found true");
}

$Case(serialisation, transform)
{
    // Deserialisation
    std::ifstream f(jsonPath);
    ag::Component c = ag::Component::FromJSON(nlohmann::json::parse(f)[0]);

    ag::Transform t = c.GetData<ag::Transform>();
    ag_expect(t.GetPosition().y == 32.0f, "Expected deserialised transform to have y pos of 32, instead found {}", t.GetPosition().y);
}