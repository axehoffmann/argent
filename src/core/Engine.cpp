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

	while (active)
	{
		/// TODO: multithreaded loop &| unblocking loop
		/// TODO: dt
		Update(0.1);
		FrameUpdate(0.1);
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
