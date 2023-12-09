#include "Query.h"

ag::IQuery::IQuery(const ComponentSet& set) : componentTypes(set)
{
}

ag::IQuery::IQuery(ag::World* w, const ComponentSet& set) : componentTypes(set), world(w)
{
    world->InitialiseQuery(this);
}


ag::IQuery::~IQuery()
{
    world->DeleteQuery(this);
}