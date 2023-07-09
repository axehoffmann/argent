#include "Query.h"

ag::IQuery::IQuery(ag::World* w, const ComponentSet& set) : world(w), componentTypes(set)
{
    world->InitialiseQuery(this);
}

ag::IQuery::~IQuery()
{
    world->DeleteQuery(this);
}