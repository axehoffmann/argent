#include "../TestFramework.h"
#include "resources/AssetManager.h"
#include "resources/Blueprint.h"

#include "core/Transform.h"
#include "rendering/StaticRenderable.h"

#include "ecs/ArchetypeCollection.h"
#include "ecs/World.h"

ag::ArchetypeCollection* archetype;
std::shared_ptr<ag::World> world;

const std::string en1Path = "assets/entity.json";
const std::string en2Path = "assets/entity2.json";


$ITest(blueprint, {});

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
    archetype = new ag::ArchetypeCollection({ ag::ComponentInfo::GetID<ag::Transform>(), ag::ComponentInfo::GetID<ag::StaticRenderable>() });
    world = std::make_shared<ag::World>();

    world->AddArchetype(archetype);

    auto query = ag::Query<ag::Transform, ag::StaticRenderable>(world.get());

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