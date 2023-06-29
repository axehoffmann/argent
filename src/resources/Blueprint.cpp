#include "Blueprint.h"

using json = nlohmann::json;

bool ag::Blueprint::IsReady()
{
	return components.size() > 0;
}

void ag::Blueprint::Load()
{
	json ob = ag::AssetManager::ReadJson(filePath);

	for (json::iterator it = ob.begin(); it != ob.end(); it++)
	{
		components.push_back(ag::Component::FromJSON(*it));
		types.push_back(components.back().GetID());
	}

	// ArchetypeCollections expect components to be sorted by ID
	std::sort(components.begin(), components.end(), [](Component& a, Component& b) -> bool
	{
		return a.GetID() < b.GetID();
	});
}

void ag::Blueprint::Unload()
{
	std::vector<ag::Component>().swap(components);
}

void ag::Blueprint::SetWorld(std::shared_ptr<ag::World> w)
{
	world = w;
	archetype = world->FindArchetype(types);
}

ag::Entity ag::Blueprint::Instantiate()
{

}
