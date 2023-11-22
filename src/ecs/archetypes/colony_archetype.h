#pragma once

#include "lib/basic.h"
#include "lib/tuple.h"
#include "lib/vector.h"

#include "ecs/archetypes/data_array_colony.h"
#include "ecs/components/component_set.h"

namespace ag
{
	class archetype
	{
	public:
		template <typename ... Ts>
		struct iterator
		{
			/**
			 * Creates an iterator across a set of archetypes
			 * @param archs the set of archetypes to iterate across
			*/
			iterator(range<archetype*>&& archs) : archetypes(archs), currentIdx(0), currentArch(0) {}

			/**
			 * Creates an end iterator to mark the end of a range
			 * @param idx		the entity count of the last archetype
			 * @param archIdx	the index of the last archetype
			*/
			iterator(u16 idx, u16 blockIdx, u16 archIdx) : currentIdx(idx), currentBlock(blockIdx), currentArch(archIdx) {}

			friend bool operator==(const iterator<Ts...>& other) const
			{
				return other.currentArch == currentArch
					&& other.currentIdx == currentIdx;
			}

			iterator& operator++()
			{
				increment();
				return *this;
			}

			iterator operator++(i32)
			{
				iterator tmp = *this;
				increment();
				return tmp;
			}

			tuple<Ts&...> operator*() const
			{
				return tuple<Ts&...>{ *(std::get<Ts*>(currentIters) + currentIdx)... };
			}

		private:
			tuple<Ts*...> currentIters;
			range<archetype*> archetypes;
			u16 currentIdx;
			u16 currentBlock;
			u16 currentArch;

			void increment()
			{
				/// TODO: use skipfield
				currentIdx++;
				if (currentIdx >= colonyBlockSize)
				{
					currentBlock++;
					currentIdx = 0;

					if (currentBlock < blockCount)
					{
						// Step to next block
						const archetype& cur = archetypes[currentArch];
						currentIters = tuple<Ts*...>{ cur.getBlockPointer<Ts>(currentBlock)... };
					}
					else
					{
						// Step to next archetype
						currentBlock = 0;
						currentArch++;

						const archetype& cur = archetypes[currentArch];
						currentIters = tuple<Ts*...>{ cur.getBlockPointer<Ts>(currentBlock)... };
					}
				}
			}
		};

	private:
		
		template <typename T>
		void getBlockPointer(u16 block) const
		{
			u8 typeIdx = componentTypes.find(componentID<T>);
			return static_cast<T*>(data.at(typeIdx).blocks.at(block)->begin);
		}

		component_set<MAX_COMPONENTS> componentTypes;

		vector<colony> data;
		skip_field skipField;

		u16 blockCount;
		u64 entityCount;
	};
}