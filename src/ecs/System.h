#pragma once

#include <memory>
#include <vector>
#include "World.h"

namespace ag
{
	class ISystem
	{
	public:
		void SetWorld(std::shared_ptr<ag::World> w);

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




		using factory_func = std::unique_ptr<ag::ISystem>();

		/**
		* Static registry of system factory functions.
		*/
		static std::vector<factory_func*>& Systems()
		{
			static std::vector<factory_func*> sysvec;
			return sysvec;
		}

		/**
		* Object for every System to statically initialise. Required for a System to be added to the ECS World.
		* Usage:
		*     class ExampleSystem {
		*	      static ag::System::SystemRegister<ExampleSystem> reg;
		*	  }
		*	  ag::System::SystemRegister<ExampleSystem> ExampleSystem::reg();
		*/
		template <class T>
		class SystemRegister
		{
		public:
			SystemRegister()
			{
				ag::ISystem::Register(Create);
			}

			static std::unique_ptr<ag::ISystem> Create()
			{
				return std::make_unique<T>();
			}
		};

	protected:

		static void Register(factory_func* func)
		{
			Systems().push_back(func);
		}

		std::shared_ptr<ag::World> world;
	};

	template <class SysType>
	class System : public ISystem
	{
		inline static ag::ISystem::SystemRegister<SysType> _reg = ag::ISystem::SystemRegister<SysType>();
	};
}