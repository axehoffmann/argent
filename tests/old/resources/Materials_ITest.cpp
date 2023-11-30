#include "../TestFramework.h"
#include "resources/AssetManager.h"
#include "resources/Material.h"

const std::string matPath = "assets/material.mat";

$ITest(materials, {});

$Cleanup(materials)
{
    ag::AssetManager::DropAll();
}

$Case(material_loading, materials)
{
    uint32_t matID = ag::AssetManager::Load<ag::Material>(matPath);

    std::weak_ptr<ag::Material> wMat = ag::AssetManager::Fetch<ag::Material>(matID);

    ag_expect(!wMat.expired(), "Expected pointer to Material resource to be valid");

    std::shared_ptr<ag::Material> mat = wMat.lock();

    ag_expect(mat->IsReady(), "Expected Material to be ready");

    ag_expect(mat->parameters[0] == 0.4f, "Expected Material parameter[0] to be 0.4, instead found {}", mat->parameters[0]);

    mat->Unload();

    ag_expect(!mat->IsReady(), "Expected Material to no longer be ready after unload");
}