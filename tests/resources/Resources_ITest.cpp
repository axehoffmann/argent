#include "../TestFramework.h"
#include "resources/AssetManager.h"

$ITest(resources, {});

$Cleanup(resources)
{
    ag::AssetManager::DropAll();
}