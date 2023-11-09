#include "World.h"

void ag::World::AddArchetype(std::shared_ptr<ArchetypeCollection>&& arch)
{
    archetypes.push_back(arch);
    archetypeIDMap[arch->GetID()] = arch.get();
    for (ag::IQuery* q : queries)
    {
        q->AddIfMatch(arch);
    }
}

void ag::World::InitialiseQuery(ag::IQuery* query)
{
    queries.push_back(query);
    for (std::shared_ptr<ArchetypeCollection> arch : archetypes)
    {
        query->AddIfMatch(arch);
    }
}

ag::World::~World()
{

}