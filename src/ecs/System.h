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




		using factory_func = std::unique_ptr<ag::System>();
		static std::vector<factory_func*>& Systems()
		{
			static std::vector<factory_func*> sysvec;
			return sysvec;
		}

		static void Register(factory_func* func)
		{
			Systems().push_back(func);
		}

		template <class T>
		class SystemRegister
		{
		public:
			SystemRegister()
			{
				ag::System::Register(Create);
			}

			static std::unique_ptr<ag::System> Create()
			{
				return std::make_unique<T>();
			}
		};

	protected:
		std::shared_ptr<ag::World> world;
	};

	
}