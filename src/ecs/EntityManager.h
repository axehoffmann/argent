#pragma once

#include "ArchetypeCollection.h"

namespace ag
{
	class EntityManager
	{
	public:
		void SpawnEntity();
		void DestroyEntity();
	
	private:
		std::vector<std::shared_ptr<ArchetypeCollection>> archetypes;
	};
}