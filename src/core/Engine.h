#pragma once

#include "rendering/scene_builder.h"
#include "rendering/ogl/renderer.h"
#include "ecs/World.h"
#include "ecs/System.h"
#include "debug/log/Log.h"

#include <memory>
#include <chrono>
#include <vector>

namespace ag
{
	class Engine
	{
	public:
		Engine();
		~Engine();

		/**
		* Begin the engine loop.
		*/
		void Run();

		/**
		* Registers a system to the ECS world.
		*/
		void RegisterSystem(ag::ISystem* s);

		void End();

	private:
		void Update(double dt);
		void FrameUpdate(double dt);

		std::unique_ptr<renderer> render;

		std::shared_ptr<ag::scene_builder> sceneBuilder;
		std::shared_ptr<ag::World> ecsWorld;

		window w;
	
		vector<ag::ISystem*> systems;

		bool active;
	};
}