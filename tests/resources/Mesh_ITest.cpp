#include "../TestFramework.h"
#include "resources/AssetManager.h"
#include "resources/Mesh.h"

const std::string meshPath = "assets/cube.obj";

$ITest(meshes, {});

$Cleanup(meshes)
{
    ag::AssetManager::DropAll();
}

$Case(mesh_loading, meshes)
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
