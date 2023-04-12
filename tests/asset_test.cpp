#include "TestFramework.h"
#include "../src/resources/AssetManager.h"
#include "../src/resources/Mesh.h"
#include "../src/resources/Texture.h"

std::string meshPath = "assets/cube.obj";
std::string texPath = "assets/frog.png";

void Setup()
{

}

void Cleanup()
{
    ag::AssetManager::UnloadAll();
}

void mesh_loading()
{
    uint32_t meshID = ag::AssetManager::Load<ag::Mesh>(meshPath);

    std::weak_ptr<ag::Mesh> wMesh = ag::AssetManager::Fetch<ag::Mesh>(meshID);

    ag_expect(!wMesh.expired(), "Expected pointer to Mesh resource to be valid");

    std::shared_ptr<ag::Mesh> mesh = wMesh.lock();

    ag_expect(mesh->IsReady(), "Expected Mesh to be ready");

    ag_expect(mesh->vertices.size() == 36, "Expected Mesh to have 36 vertices, instead found {}", mesh->vertices.size());

    mesh->Unload();

    ag_expect(!mesh->IsReady(), "Expected Mesh to no longer be ready after unload");
}


void texture_loading()
{

}

int main()
{
    Test::Name("Assets");
    Test::init = Setup;
    Test::clean = Cleanup;

    Test::Case("Mesh Loading", mesh_loading);
    Test::Case("Texture Loading", texture_loading);

    Test::Run();
}