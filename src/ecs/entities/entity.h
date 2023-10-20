#pragma once

#include "ecs/ArchetypeCollection.h"

namespace ag
{
	/**
	* A temporary handle to an entity
	*/
	template <typename ... Cs>
	struct entity
	{


	private:
		size_t index;
		ArchetypeCollection* arch;
	};
}