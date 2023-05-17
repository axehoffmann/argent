#pragma once

#include <memory>
#include <vector>
#include "World.h"

namespace ag
{
	class System
	{
	public:
		void Create(std::shared_ptr<ag::World> w);

		/**
		* Called when all ECS systems are initialised at client startup.
		*/
		virtual void Init() = 0;
		/**
		* Called every game update. dt is unchanging between updates.
		* @param dt The time since last update. Constant value
		*/
		virtual void Update(double dt) = 0;
		/**
		* Called every frame. dt is dependent on monitor rates and/or performance
		* @param dt The time since last frame
		*/
		virtual void FrameUpdate(double dt) = 0;

		static std::vector<ag::System*> systems;
	protected:
		std::shared_ptr<ag::World> world;
	};

	template <class T>
	class SystemRegister
	{
		SystemRegister()
		{
			ag::System::systems.push_back(new T());
		}
	};
}