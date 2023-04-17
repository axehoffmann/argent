#include "World.h"

void ag::World::AddArchetype(ag::ArchetypeCollection* arch)
{
    archetypes.push_back(arch);
}

ag::World::~World()
{
	for (size_t i = 0; i < archetypes.size(); i++)
	{
		delete archetypes[i];
	}

	std::vector<ag::ArchetypeCollection*>().swap(archetypes);
}