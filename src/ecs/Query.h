#pragma once

#include "ECSTypes.h"
#include "World.h"
#include "ArchetypeCollection.h"

#include <vector>
#include <tuple>
#include <algorithm>
#include <type_traits>
#include <numeric>

namespace ag
{
    class World;
    class IQuery
    {
    public:
        /**
        * Adds an archetype to this Query if it matches the Query's component set.
        */
        void AddIfMatch(std::shared_ptr<ag::ArchetypeCollection> archetype)
        {
            if (Matches(archetype->GetComponentSet()))
                matches.push_back(archetype);
        }

        IQuery(ag::World* w, const ComponentSet& set);

        /**
        * Unsubscribe from getting archetype updates
        */
        ~IQuery();

        /**
        * Calculates total count of matched entities.
        */
        size_t Size() 
        {
            return std::accumulate(matches.begin(), matches.end(), 0,
                [](size_t sum, std::shared_ptr<ag::ArchetypeCollection> b) {
                return sum + b->GetEntityCount();
            });
        }


    protected:
        bool Matches(const ComponentSet& set)
        {
            return std::includes(set.begin(), set.end(), componentTypes.begin(), componentTypes.end());
        }

        std::vector<std::shared_ptr<ag::ArchetypeCollection>> matches;
        ag::World* world;

        /// TODO: consider a small-size optimised hash set implementation for component set comparisons. 
        /// Probably uneccesary in this case though. https://www.youtube.com/watch?v=vElZc6zSIXM
        ComponentSet componentTypes;
    };

    template <typename... ComponentTypes>
    class Query : public IQuery 
    {
        friend class ag::World;
    public:

        Query(ag::World* w) : IQuery(w, ag::ComponentInfo::GetComponentSet<ComponentTypes...>()) {}

        class Iterator
        {
            friend class Query;

        public:
            Query::Entity operator*()
            {
                return Query::Entity(currentArchetype.get(), index);
            }

            bool operator!=(const Iterator& other) const
            {
                return currentArchetype != other.currentArchetype || index != other.index;
            }

            Iterator& operator++()
            {
                index++;

                if (index >= currentArchetypeSize && archIndex + 1 < query->matches.size())
                {
                    index = 0;
                    archIndex++;
                    currentArchetype = query->matches[archIndex];
                    currentArchetypeSize = currentArchetype->GetEntityCount();
                }

                return *this;
            }


        private:
            Iterator(Query<ComponentTypes...>* q, size_t arch, size_t i) : 
                query(q), archIndex(arch), index(i),
                currentArchetype(q->matches[arch]), 
                currentArchetypeSize(currentArchetype->GetEntityCount()) {}

            std::shared_ptr<ag::ArchetypeCollection> currentArchetype;
            size_t archIndex; // Index of the current Archetype within the parent Query's 'matches' array.
            size_t index; // Index of the iterator WITHIN the current archetype. Once this exceeds the current Archetype's bounds, we move to the next archetype in the query.
            size_t currentArchetypeSize; // We cache this so we don't go through 3 function calls to get the archetype's size every time we iterate.
            Query<ComponentTypes...>* query;
        };

        struct Entity
        {
            friend class Query::Iterator;

            template <typename C>
            C& Get()
            {
                // Component we are getting must be one of the types from the Query this entity is from
                static_assert((std::is_same_v<C, ComponentTypes> || ...));
                return archetype->GetComponent<C>(index);
            }

            EntityInfo Info()
            {
                return archetype->GetEntityInfo(index);
            }

            // This should not be copyable and cannot persist outside of a Query iteration
            // because after entities are added or deleted this may no longer point to
            // the same entity.
            Query::Entity(const Query::Entity&) = delete;
            Query::Entity& operator=(const Query::Entity&) = delete;

        private:
            Entity(ag::ArchetypeCollection* a, size_t i) : archetype(a), index(i) {}

            ag::ArchetypeCollection* archetype;
            size_t index;
        };

        Iterator begin()
        {
            return Iterator(this, 0, 0);
        }

        Iterator end()
        {
            return Iterator(this, matches.size() - 1, matches.back()->GetEntityCount());
        }
    };
}