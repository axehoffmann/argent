#pragma once

#include "AssetManager.h"
#include "Resource.h"
#include "../ecs/Component.h"
#include <nlohmann/json.hpp>

#include "lib/functor.h"
#include "lib/vector.h"

#include "ecs/World.h"
#include "ecs/EntityRef.h"
#include "ecs/ArchetypeCollection.h"

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

		template <typename ... Ts>
		ag::EntityRef Instantiate(function<Ts*...> f)
		{
			EntityID id = archetype->SpawnEntity(components, f);
			return ag::EntityRef(id, world.get());
		}

		ag::EntityRef Instantiate()
		{
			EntityID id = archetype->SpawnEntity(components);
			return ag::EntityRef(id, world.get());
		}


	private:
		vector<ComponentTypeID> types;
		vector<ag::Component> components;

		// Stores the targeted ECS world and archetype.
		std::shared_ptr<ag::World> world;
		std::shared_ptr<ag::ArchetypeCollection> archetype;
	};
}