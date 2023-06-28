#include "Material.h"

/// TODO: Implement
bool ag::Material::IsReady()
{
	return parameters.size() > 0 || textures.size() > 0;
}

void ag::Material::Load()
{
	std::ifstream matFile(filePath);
	json matData = json::parse(matFile);

	std::vector<std::string> texPaths = matData["textures"].get<std::vector<std::string>>();

	// TODO: make parameters more customisable eg vec1-4, name, etc
	parameters = matData["parameters"].get<std::vector<float>>();

	for (size_t i = 0; i < texPaths.size(); i++)
	{
		textures.push_back(ag::AssetManager::Load<ag::Texture>(texPaths[i]));
	}
}

void ag::Material::Unload()
{
	// TODO: material data should theoretically be minimal. may not need to unload.
	std::vector<float>().swap(parameters);
	std::vector<uint32_t>().swap(textures);
}