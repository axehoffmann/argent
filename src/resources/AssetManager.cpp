#include "AssetManager.h"

vector<std::shared_ptr<ag::Resource>> ag::AssetManager::resources;
std::unordered_map<std::string, uint32_t> ag::AssetManager::pathToID;
