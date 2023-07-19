#include "Engine.h"

ag::Engine::Engine()
{
	active = true;

	ecsWorld = std::make_shared<ag::World>();
	sceneBuilder = std::make_shared<ag::SceneBuilder>(ecsWorld);

	renderer = std::make_unique<ag::Renderer>(std::make_unique<ag::GLRenderEngine>(), sceneBuilder);
}

ag::Engine::~Engine()
{

}

void ag::Engine::Run()
{
	/// TODO: may want to research MT loops in future, don't block frames behind physics etc

	Update(0);

	Log<Engine>().Trace("Starting loop...");

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
	s->SetWorld(std::shared_ptr<ag::World>(ecsWorld));
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

	ag::Stats::RegisterFrameTime(dt);
	renderer->Render();
}
