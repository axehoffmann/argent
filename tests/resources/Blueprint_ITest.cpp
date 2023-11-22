/*
#include "../TestFramework.h"
#include "resources/AssetManager.h"
#include "resources/Blueprint.h"

#include "core/Transform.h"
#include "rendering/StaticRenderable.h"

#include "ecs/ArchetypeCollection.h"
#include "ecs/World.h"

namespace blueprint_test
{
    const std::string en1Path = "assets/entity.json";
    const std::string en2Path = "assets/entity2.json";

    $ITest(blueprint, {});
    
    struct TestComponent
    {
        int value;
        static nlohmann::json ToJSON(TestComponent t)
        {
            nlohmann::json ob;
            ob["type"] = "test_blueprint_component";
            ob["value"] = t.value;
            return ob;
        }
        static TestComponent FromJSON(nlohmann::json& ob)
        {
            return TestComponent{ .value = ob["value"] };
        }
        static ag::Component::Serialiser<TestComponent> serialiser;
    };

    ag::Component::Serialiser<TestComponent> TestComponent::serialiser("test_blueprint_component");

    $Cleanup(blueprint)
    {
        ag::AssetManager::DropAll();
    }

    $Case(loading, blueprint)
    {
        uint32_t bpID = ag::AssetManager::Load<ag::Blueprint>(en1Path);

        std::weak_ptr<ag::Blueprint> wBp = ag::AssetManager::Fetch<ag::Blueprint>(bpID);
        ag_expect(!wBp.expired(), "Expected pointer to Blueprint resource to be valid");

        std::shared_ptr<ag::Blueprint> bp = wBp.lock();
        ag_expect(bp->IsReady(), "Expected Blueprint to be ready");

        bp->Unload();
        ag_expect(!bp->IsReady(), "Expected Blueprint to no longer be ready after unload");
    }

    $Case(instantiation, blueprint)
    {
        std::shared_ptr<ag::ArchetypeCollection> archetype = std::make_shared<ag::ArchetypeCollection>(ComponentSet{ ag::ComponentInfo::GetID<ag::Transform>(), ag::ComponentInfo::GetID<TestComponent>() });
        std::shared_ptr<ag::World> world = std::make_shared<ag::World>();

        world->AddArchetype(std::move(archetype));

        auto query = ag::Query<ag::Transform, TestComponent>(world.get());

        uint32_t bpID = ag::AssetManager::Load<ag::Blueprint>(en1Path);
        std::weak_ptr<ag::Blueprint> wBp = ag::AssetManager::Fetch<ag::Blueprint>(bpID);
        ag_expect(!wBp.expired(), "Expected pointer to Blueprint resource to be valid");

        std::shared_ptr<ag::Blueprint> bp = wBp.lock();
        ag_expect(bp->IsReady(), "Expected Blueprint to be ready");

        ag_expect(query.Size() == 0, "Expected an empty query before the blueprint was instantiated");


        bp->SetWorld(world);
        bp->Instantiate();

        archetype->ResolveBuffers();

        ag_expect(query.Size() == 1, "Expected a single entity after the blueprint was instantiated");

        ag_expect((*query.begin()).Get<ag::Transform>().GetPosition().y == 32.0f,
            "Expected deserialised transform to have y pos of 32, instead found {}",
            (*query.begin()).Get<ag::Transform>().GetPosition().y);

    }
}
*/