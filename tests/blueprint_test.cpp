#include "TestFramework.h"
#include "../src/resources/AssetManager.h"
#include "../src/resources/Blueprint.h"

#include "../src/core/Transform.h"
#include "../src/rendering/StaticRenderable.h"

#include "../src/ecs/ArchetypeCollection.h"
#include "../src/ecs/World.h"

ag::ArchetypeCollection* archetype;
ag::World* world;

const std::string en1Path = "assets/entity.json";
const std::string en2Path = "assets/entity2.json";


void Cleanup()
{
    ag::AssetManager::UnloadAll();
}

void loading()
{
    uint32_t bpID = ag::AssetManager::Load<ag::Blueprint>(en1Path);

    std::weak_ptr<ag::Blueprint> wBp = ag::AssetManager::Fetch<ag::Blueprint>(bpID);
    ag_expect(!wBp.expired(), "Expected pointer to Blueprint resource to be valid");

    std::shared_ptr<ag::Blueprint> bp = wBp.lock();
    ag_expect(bp->IsReady(), "Expected Blueprint to be ready");

    bp->Unload();
    ag_expect(!bp->IsReady(), "Expected Blueprint to no longer be ready after unload");
}

void instantiation()
{
    archetype = new ag::ArchetypeCollection({ ag::ComponentInfo::GetID<ag::Transform>(), ag::ComponentInfo::GetID<ag::StaticRenderable>() });
    world = new ag::World();

    world->AddArchetype(archetype);

    uint32_t bpID = ag::AssetManager::Load<ag::Blueprint>(en1Path);
    std::weak_ptr<ag::Blueprint> wBp = ag::AssetManager::Fetch<ag::Blueprint>(bpID);
    ag_expect(!wBp.expired(), "Expected pointer to Blueprint resource to be valid");

    std::shared_ptr<ag::Blueprint> bp = wBp.lock();
    ag_expect(bp->IsReady(), "Expected Blueprint to be ready");

    std::function<void(ag::QueryResult<ag::Transform>, ag::QueryResult<ag::StaticRenderable>)> lm = [](ag::QueryResult<ag::Transform> t, ag::QueryResult<ag::StaticRenderable> s) {
        ag_expect(t.Length() == 0, "Expected an empty query before the blueprint was instantiated");
    };
    world->Query(lm);

    bp->SetWorld(std::shared_ptr<ag::World>(world));
    bp->Instantiate();

    archetype->ResolveBuffers();

    lm = [](ag::QueryResult<ag::Transform> t, ag::QueryResult<ag::StaticRenderable> s) {
        ag_expect(t.Length() == 1, "Expected a single entity after the blueprint was instantiated");

        ag_expect(t[0].GetPosition().y == 32.0f, "Expected deserialised transform to have y pos of 32, instead found {}", t[0].GetPosition().y);


    };
    world->Query(lm);

    delete world;
}


int main()
{
    Test::Name("Blueprints");

    Test::clean = Cleanup;

    Test::Case("Loading", loading);
    Test::Case("Instantiation", instantiation);

    Test::Run();
}