#pragma once

#include "resources.h"
#include "resrc_handle.h"
#include "resrc_specifier.h"

#include "lib/string.h"
#include "lib/map.h"
#include "lib/vector.h"
#include "lib/queue.h"
#include "lib/sync.h"
#include "lib/tuple.h"

namespace ag
{
	class resrc_manager
	{
	public:
		/**
		 * Performs a blocking load for a resource using the current thread
		 * @tparam T the type of resource to load
		 * @param spec the resource's specifier
		 * @return a handle to the loaded resource
		*/
		template <class T>
		resrc_handle<T> load_immediate(resrc_specifier<T> spec)
		{
			entry* resrc = nullptr;
			bool present = resources.operate([&](map<string, entry>& rm)
				{
					auto iter = rm.find(spec.path);
					if (iter != rm.end())
					{
						resrc = iter->second;
						return true;
					}

					rm.emplace(spec.path, { 0, new T });
					return false;
				});

			// There are 3 cases to handle.
			// 1. we have to load the resource ourselves
			if (!present)
			{
				resrc->status = resrc->data->load(spec.path);
				return { &resrc->status, static_cast<T*>(resrc->data) };
			}

			// 2. the resource is already loaded
			// 3. someone is loading the resource right now, and we wait on them
			/// TODO: prove this won't result in locked threads for any resource type.
			while (resrc.status == 0);

			return { &resrc->status, static_cast<T*>(resrc->data) };
		}

		template <class T>
		resrc_handle<T> load_async(resrc_specifier<T> spec)
		{
			entry* resrc = nullptr;
			bool present = resources.operate([&](map<string, entry>& rm)
				{
					auto iter = rm.find(spec.path);
					if (iter != rm.end())
					{
						resrc = iter->second;
						return true;
					}

					rm.emplace(spec.path, { 0, new T });
					return false;
				});

			if (!present)
			{
				work_queue.operate([&](queue<pair<string, entry*>>& q)
					{
						q.push({ spec.path, resrc });
					});
			}

			return { &resrc->status, static_cast<T*>(resrc->data) };
		}

		/**
		 * Puts the current thread to work loading a single resource. Returns immediately if no tasks are available.
		*/
		void do_work()
		{
			string current_job = "";
			entry* resrc = nullptr;

			// Pop a task from the queue
			work_queue.operate([&](queue<pair<string, entry*>>& q)
			{
				if (!q.empty())
				{
					tie(current_job, resrc) = q.front();
					q.pop();
				}
			});

			// Load the resource
			if (resrc != nullptr)
				resrc->status = resrc->data->load(current_job);
		}

	private:
		/// TODO: status ints won't have consistent position depending on map implementation
		struct entry
		{
			atomic<i8> status;
			Resource* data;
		};

		locked<map<string, entry>> resources;

		locked<queue<pair<string, entry*>>> work_queue;

	};
}