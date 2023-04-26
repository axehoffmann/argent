#pragma once

#include <memory>
#include "World.h"

namespace ag
{
	class System
	{
	public:
		void Create(std::shared_ptr<ag::World> w);

		virtual void Init() = 0;
		virtual void Update(double dt) = 0;
		virtual void FrameUpdate(double dt) = 0;


	protected:
		std::shared_ptr<ag::World> world;
	};
}