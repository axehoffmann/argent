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
    uint32_t texID = ag::AssetManager::Load<ag::Texture>(texPath);

    std::weak_ptr<ag::Texture> wTex = ag::AssetManager::Fetch<ag::Texture>(texID);

    ag_expect(!wTex.expired(), "Expected pointer to Texture resource to be valid");

    std::shared_ptr<ag::Texture> tex = wTex.lock();

    ag_expect(tex->IsReady(), "Expected Texture to be ready");

    ag_expect(tex->width == 10, "Expected Texture to be 10 pixels wide, instead found {}", tex->width);

    tex->Unload();

    ag_expect(!tex->IsReady(), "Expected Texture to no longer be ready after unload");
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