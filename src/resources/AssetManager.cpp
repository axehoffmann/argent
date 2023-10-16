#include "AssetManager.h"

vector<std::shared_ptr<ag::Resource>> ag::AssetManager::resources;
map<std::string, uint32_t> ag::AssetManager::pathToID;
