#include "Engine.h"

#include "resources/Blueprint.h"
#include "resources/Mesh.h"

ag::Engine::Engine()
{
	active = true;

	ecsWorld = std::make_shared<ag::World>();
	sceneBuilder = std::make_shared<ag::scene_builder>(ecsWorld);

	render = std::make_unique<renderer>();
	render->createRenderable(AssetManager::Load<Mesh>("assets/pillar.obj"));
	render->createRenderable(AssetManager::Load<Mesh>("assets/cube.obj"));


	/*
	uint32_t cube = AssetManager::Load<Blueprint>("assets/entities/cube.json");
	uint32_t l1 = AssetManager::Load<Blueprint>("assets/entities/l1.json");

	auto cb = AssetManager::Fetch<Blueprint>(cube).lock();
	cb->SetWorld(ecsWorld);
	cb->Instantiate();

	auto l = AssetManager::Fetch<Blueprint>(l1).lock();
	l->SetWorld(ecsWorld);
	l->Instantiate();
	*/
}

ag::Engine::~Engine()
{

}

void ag::Engine::Run()
{
	using namespace std::chrono_literals;
	namespace stc = std::chrono;
	/// TODO: may want to research MT loops in future, don't block frames behind physics etc

	Update(0);

	Log::Trace("Starting loop... ");

	// Based on Fix your Timestep! by Glenn Fiedler
	stc::nanoseconds t(0);
	// TODO: proper 1/60? research better chrono stuff
	stc::nanoseconds dt = stc::milliseconds(167);

	auto currentTime = stc::steady_clock::now();
	stc::nanoseconds accumulator(0);

	while (active)
	{
		auto newTime = stc::steady_clock::now();
		auto frameTime = newTime - currentTime;

		if (frameTime > 250ms)
			frameTime = 250ms;
		currentTime = newTime;

		accumulator += frameTime;

		while (accumulator >= dt)
		{
			Update(stc::duration<double>(dt).count());
			accumulator -= dt;
			t += dt;
		}

		/// TODO: is frame time correct step here? unsure
		FrameUpdate(stc::duration<double>(frameTime).count());
	}
}

void ag::Engine::RegisterSystem(ag::ISystem* s)
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

	ecsWorld->ResolveBuffers();
}

void ag::Engine::FrameUpdate(double dt)
{
	for (size_t i = 0; i < systems.size(); i++)
	{
		systems[i]->FrameUpdate(dt);
	}

	ag::Stats::RegisterFrameTime(dt);
	render->render(sceneBuilder->StartGraphRead());
	sceneBuilder->EndGraphRead();
	w.swapBuffers();
}
