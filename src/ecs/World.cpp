#include "World.h"

void ag::World::AddArchetype(ag::ArchetypeCollection* arch)
{
    archetypes.push_back(std::shared_ptr<ag::ArchetypeCollection>(arch));
}

ag::World::~World()
{
	std::vector<std::shared_ptr<ag::ArchetypeCollection>>().swap(archetypes);
}