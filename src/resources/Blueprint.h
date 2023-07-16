#pragma once

#include "AssetManager.h"
#include "Resource.h"
#include "../ecs/Component.h"
#include <nlohmann/json.hpp>
#include <vector>

#include "../ecs/Entity.h"
#include "../ecs/World.h"
#include "../ecs/ArchetypeCollection.h"

namespace ag
{
	class Blueprint : public Resource
	{
	public:

		Blueprint(std::string path) : Resource(path) {}
		~Blueprint() { Unload(); }

		bool IsReady() const override;
		void Load() override;
		void Unload() override;

		void SetWorld(std::shared_ptr<ag::World> w);
		ag::Entity Instantiate();

	private:
		std::vector<ComponentTypeID> types;
		std::vector<ag::Component> components;

		// Stores the targeted ECS world and archetype.
		std::shared_ptr<ag::World> world;
		std::shared_ptr<ag::ArchetypeCollection> archetype;
	};
}