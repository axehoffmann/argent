#include "../TestFramework.h"
#include "resources/AssetManager.h"
#include "resources/Texture.h"

const std::string texPath = "assets/frog.png";

$ITest(textures, {});

$Cleanup(textures)
{
    ag::AssetManager::DropAll();
}

$Case(texture_loading, textures)
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