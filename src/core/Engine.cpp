#include "Engine.h"

#include "resources/Blueprint.h"
#include "resources/Mesh.h"
#include "resources/Texture.h"


class test_system : public ag::System<test_system>
{
public:
	test_system() : ag::System<test_system>(), q() {}

	void Init() override
	{
		world->InitialiseQuery(&q);
	}
	void Update(f64 dt) override
	{
		for (auto e : q)
		{
			auto& t = e.Get<transform>();

			if (t.scale.x > 5) continue;

			t.rot *= glm::quat({0, 0.001, 0});
		}
	}

	ag::Query<transform> q;
};

ag::Engine::Engine()
{
	namespace stc = std::chrono;

	active = true;

	ecsWorld = std::make_shared<ag::World>();
	sceneBuilder = std::make_shared<ag::scene_builder>(ecsWorld, meshInfo);
	render = std::make_unique<renderer>(meshInfo);

	auto t = stc::steady_clock::now();
	/*
	render->createRenderable(AssetManager::Load<Mesh>("assets/cube/cube2.agmesh"));
	render->createRenderable(AssetManager::Load<Mesh>("assets/cliff/cliff.agmesh"));
	render->createRenderable(AssetManager::Load<Mesh>("assets/pillar/pillar.agmesh"));

	render->loadMaterial(0, { 
		AssetManager::Load<Texture>("assets/pillar/pillar1.png"), 
		AssetManager::Load<Texture>("assets/pillar/pillar_norm.png"),
		AssetManager::Load<Texture>("assets/pillar/pillar_detail.png")
	});
	render->loadMaterial(1, { 
		AssetManager::Load<Texture>("assets/cube/pepe.png"),
		AssetManager::Load<Texture>("assets/cube/pepe_norm.png"),
		AssetManager::Load<Texture>("assets/cube/pepe_detail.png") 
	});
	render->loadMaterial(2, {
		AssetManager::Load<Texture>("assets/cliff/cliff_alb.png"),
		AssetManager::Load<Texture>("assets/cliff/cliff_norm.png"),
		AssetManager::Load<Texture>("assets/cliff/cliff_detail.png")
	});

	u32 cube = AssetManager::Load<Blueprint>("assets/entities/cube.json");
	u32 pilar = AssetManager::Load<Blueprint>("assets/entities/pillar.json");
	u32 cliff = AssetManager::Load<Blueprint>("assets/entities/cliff.json");

	auto cb = AssetManager::Fetch<Blueprint>(cube).lock();
	cb->SetWorld(ecsWorld);

	auto l = AssetManager::Fetch<Blueprint>(pilar).lock();
	l->SetWorld(ecsWorld);

	auto r = AssetManager::Fetch<Blueprint>(cliff).lock();
	r->SetWorld(ecsWorld);
	*/
	Log::Trace(sfmt("Loading: {}ms", (stc::steady_clock::now() - t).count() / 1e6));

	t = stc::steady_clock::now();
	/*
	i32 state = 0;
	function<transform*> init = [&](transform* tp) 
	{
		transform& t = *tp;
		t.pos.y = state % 4 == 0 ? -16 : -15;
		t.pos.x = 3 * ((state % 1040) % 96) - 150;
		t.pos.z = -40 -  2 * ((state % 1040) / 96);
		t.pos.y += (state / 1040) * 10.0f;
			
		t.rot *= glm::quat({ 0, 0.013 * state, 0 });

		state += 2;
	};
	for (size_t i = 0; i < 2000; i++)
	{
		l->Instantiate(init);
		cb->Instantiate(init);
	}

	function<transform*> i2 = [](transform* tp)
	{
		transform& t = *tp;
		t.scale = { 5, -5, 5};
		t.pos = { 0, -20, -60};
		t.rot = { 0, -1, 0, 0};
	};
	// cb->Instantiate(i2);
	r->Instantiate(i2);

	*/
	Log::Trace(sfmt("Initialisation: {}ms", (stc::steady_clock::now() - t).count() / 1e6));

	RegisterSystem(new test_system);
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
	stc::nanoseconds dt = stc::milliseconds(17);

	auto currentTime = stc::steady_clock::now();
	stc::nanoseconds accumulator(0);

	u64 frame = 0;
	double ftime = 0;

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

		w.pollEvents();
		/// TODO: is frame time correct step here? unsure
		FrameUpdate(stc::duration<double>(frameTime).count());
		frame++;
		ftime += double(frameTime.count()) / 1000000000.0;

		if (ftime >= 5.0)
		{
			Log::Trace(sfmt("time: {}ms FPS: {}", 
				std::round(ftime * 10000000.0 / frame) / 10000.0, 
				std::round(frame * 10000.0 / ftime) / 10000.0));
			ftime = 0;
			frame = 0;
		}
	}
}

void ag::Engine::RegisterSystem(ag::ISystem* s)
{
	systems.push_back(s);
	s->SetWorld(ecsWorld.get());
	s->Init();
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
	render->render(sceneBuilder->StartGraphRead(), dt);
	sceneBuilder->EndGraphRead();
	w.swapBuffers();
}
