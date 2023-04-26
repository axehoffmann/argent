#pragma once

#include "../rendering/SceneBuilder.h"
#include "../rendering/Renderer.h"
#include "../rendering/opengl/GLRenderEngine.h"
#include "../ecs/World.h"
#include "../ecs/System.h"

#include <chrono>
#include <vector>

namespace ag
{
	class Engine
	{
	public:
		Engine();
		~Engine();

		void Run();

		void RegisterSystem(ag::System* s);

		void End();

	private:
		void Update(double dt);
		void FrameUpdate(double dt);

		ag::Renderer* renderer;
		ag::RenderEngine* renderEngine;

		ag::SceneBuilder* sceneBuilder;
		ag::World* ecsWorld;

		std::vector<ag::System*> systems;

		bool active;
	};
}