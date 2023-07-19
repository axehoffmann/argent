#pragma once

#include "rendering/SceneBuilder.h"
#include "rendering/Renderer.h"
#include "rendering/opengl/GLRenderEngine.h"
#include "ecs/World.h"
#include "ecs/System.h"
#include "core/log/Log.h"

#include "Stats.h"

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
		void RegisterSystem(ag::System* s);

		void End();

	private:
		void Update(double dt);
		void FrameUpdate(double dt);

		std::unique_ptr<ag::Renderer> renderer;

		std::shared_ptr<ag::SceneBuilder> sceneBuilder;
		std::shared_ptr<ag::World> ecsWorld;

		std::vector<ag::System*> systems;

		bool active;
	};
}