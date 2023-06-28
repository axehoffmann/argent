#pragma once

#include "AssetManager.h"
#include "Resource.h"
#include "../ecs/Component.h"
#include <nlohmann/json.hpp>
#include <vector>

namespace ag
{
	class Blueprint : public Resource
	{
	public:

		Blueprint(std::string path) : Resource(path) {}
		~Blueprint() { Unload(); }

		bool IsReady();
		void Load();
		void Unload();
	private:
		std::vector<ag::Component> components;
	};
}