#pragma once

#include "ecs/components/component_set.h"
#include "ecs/archetype.h"

#include "lib/ptr.h"
#include "lib/vector.h"

namespace ag
{
/// TODO: synchronisation if creating entities across multiple threads of same archetype
	class query_tree
	{
	public:
		archetype* find_archetype(const component_set<MAX_COMPONENTS>& cTypes)
		{
			
		}

		archetype* archetype_from_id() const
		{
			
		}

		/// TODO: Our query tree is actually equal to the naive DAG generated by system_scheduler.
		// Need a way to easily transform format or something
		struct node
		{
			vector<archetype*> archetypes;
			vector<node*> children;
		};


		~query_tree()
		{
			for (node* n : nodes)
			{
				delete n;
			}

			for (archetype* a : archetypes)
			{
				delete a;
			}
		}

		vector<node*> nodes;
		vector<archetype*> archetypes;
	private:
		
	};
}