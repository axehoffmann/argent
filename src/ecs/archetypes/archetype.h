#pragma once

#include "ecs/components/component_set.h"
#include "ecs/archetypes/data_array.h"
#include "ecs/archetypes/data_array_factory.h"

#include "lib/mt/mt_list.h"
#include "lib/mt/te_list.h"
#include "lib/ptr.h"
#include "lib/tuple.h"
#include "lib/vector.h"

namespace ag
{
	class archetype
	{
	public:

		archetype(const component_set<MAX_COMPONENTS>& cTypes, block_allocator* alloc) noexcept 
			: componentTypes(cTypes),
			  dataArrays(create_arrays_for_components(cTypes)),
			  entityCount(0),
			  destroyBuffer(2, alloc),
			  allocator(alloc)
		{
			
		}

		u64 count() const
		{
			return entityCount;
		}

		/// TODO: bad API
		template <typename ... Ts>
		void instantiateImmediate(Ts... params)
		{
			u8 i = 0;
			entityCount += 1;
			((dataArrays.at(i++)->insert(reinterpret_cast<byte*>(&params), sizeof(Ts))), ...);
		}

		void resolveBuffers()
		{
			// Erase all data in the destroy buffers
			dyn_array<u64> toDelete(destroyBuffer.size(), allocator);
			destroyBuffer.copy_to(toDelete.begin());

			for (auto& arr : dataArrays)
				arr->eraseIndices(toDelete);

			entityCount -= destroyBuffer.size();
			destroyBuffer.reset();
		}

		/**
		 * An iterator that can iterate across multiple archetypes,
		 * as long as each archetype has a subset of components Ts...
		 * @tparam ...Ts the subset of components that this iterator accesses
		*/
		template <typename ... Ts>
		struct iterator 
		{
		private:
			tuple<Ts*...> currentIters;
			u64 currentIdx;
			u64 currentArch;

			/// TODO: use span?
			vector<archetype*> archetypes;

			void increment()
			{
				currentIdx++;
				if (currentIdx >= archetypes[currentArch]->entityCount)
				{
					// Step to next archetype we iterate across
					currentArch++;
					currentIdx = 0;

					if (currentArch >= archetypes.size())
					{
						// Reached end.
						return;
					}

					updateIters();
				}
			}

			void updateIters()
			{
				currentIters = tuple<Ts*...>{ archetypes[currentArch]->get_begin<Ts>()... };
			}

		public:
			/**
			 * Creates an iterator across a set of archetypes
			 * @param archs the set of archetypes to iterate across
			*/
			iterator(vector<archetype*>&& archs) : archetypes(archs), currentIdx(0), currentArch(0) 
			{
				updateIters();
			}
			
			/**
			 * Creates an end iterator to mark the end of a range
			 * @param idx		the entity count of the last archetype
			 * @param archIdx	the index of the last archetype 
			*/
			iterator(u64 idx, u64 archIdx) : currentIdx(idx), currentArch(archIdx) {}

			bool operator==(const archetype::iterator<Ts...>& other) const
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
		};

		/**
		 * Marks the entity at the index for destruction
		 * @param idx the index of the entity
		*/
		void destroyAtIndex(u64 idx)
		{
			destroyBuffer.push(u64{idx});
		}

	private:
		/**
		 * Gets a pointer to the first element of a data array in this archetype
		 * @tparam T the type of component. Must be a valid type in this archetype
		 * @return	 a pointer to the first element of type T
		*/
		template <typename T>
		T* get_begin() const
		{
			u8 typeIdx = componentTypes.find(componentID<T>);
			return dataArrays.at(typeIdx)->begin<T>();
		}

		component_set<MAX_COMPONENTS> componentTypes;
		vector<ptr<data_array>> dataArrays;

		vector<ptr<mtte_list>> spawnBuffers;
		
		mt_list<u64> destroyBuffer;

		u64 entityCount;

		/* Short-lived allocator for frame-lifetime memory management */
		block_allocator* allocator;
	};
}