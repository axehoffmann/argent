#include "Engine.h"

ag::Engine::Engine()
{
	active = true;

	ecsWorld = new ag::World();
	sceneBuilder = new ag::SceneBuilder(ecsWorld);
	renderEngine = new ag::GLRenderEngine();
	renderer = new ag::Renderer(renderEngine, sceneBuilder);

	/// TODO: handle errors
	renderer->Initialise();
}

ag::Engine::~Engine()
{
	/// TODO: this beats the point of smart pointers.
	delete renderer;
	delete renderEngine;
	delete sceneBuilder;
	delete ecsWorld;
}

void ag::Engine::Run()
{
	/// TODO: may want to research MT loops in future, don't block frames behind physics etc

	// Based on Fix your Timestep! by Glenn Fiedler
	std::chrono::nanoseconds t(0);
	// TODO: proper 1/60? research better chrono stuff
	std::chrono::nanoseconds dt = std::chrono::milliseconds(167);

	auto currentTime = std::chrono::steady_clock::now();
	std::chrono::nanoseconds accumulator(0);

	while (active)
	{
		auto newTime = std::chrono::steady_clock::now();
		auto frameTime = newTime - currentTime;

		if (frameTime > std::chrono::milliseconds(250))
			frameTime = std::chrono::milliseconds(250);
		currentTime = newTime;

		accumulator += frameTime;

		while (accumulator >= dt)
		{
			Update(std::chrono::duration<double>(dt).count());
			accumulator -= dt;
			t += dt;
		}

		/// TODO: is frame time correct step here? unsure
		FrameUpdate(std::chrono::duration<double>(frameTime).count());
	}
}

void ag::Engine::RegisterSystem(ag::System* s)
{
	systems.push_back(s);
	s->Create(std::shared_ptr<ag::World>(ecsWorld));
}

void ag::Engine::End()
{
	active = false;
}

void ag::Engine::Update(double dt)
{
	for (size_t i = 0; i < systems.size(); i++)
	{
		systems[i]->Update(dt);
	}

	sceneBuilder->Update();
}

void ag::Engine::FrameUpdate(double dt)
{
	for (size_t i = 0; i < systems.size(); i++)
	{
		systems[i]->FrameUpdate(dt);
	}

	renderer->Render();
}
