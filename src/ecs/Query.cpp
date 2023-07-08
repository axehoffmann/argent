#include "Query.h"

ag::IQuery::IQuery(ag::World* w) : world(w)
{
    world->InitialiseQuery(this);
}

ag::IQuery::~IQuery()
{
    world->DeleteQuery(this);
}