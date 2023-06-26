#pragma once

#include "ECS.h"
#include "../resources/AssetManager.h"
#include "Component.h"

#include <string>
#include <vector>
#include <algorithm>

namespace ag
{
	class EntityTemplate
	{
	public:

		static EntityTemplate FromJSON(std::string path)
		{
			using json = nlohmann::json;
			json ob = ag::AssetManager::ReadJson(path);

			for (json::iterator it = conf.begin(); it != conf.end(); it++)
			{
				data.push_back(Component::FromJSON(*it));
			}

			// ArchetypeCollections expect components to be sorted by ID
			std::sort(data.begin(), data.end(), [](const Component& a, const Component& b) -> bool
			{
				return a.GetID() < b.GetID();
			});
		}

	private:
		std::vector<Component> data;
	};
}