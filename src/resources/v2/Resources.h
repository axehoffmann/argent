#pragma once

#include <string>
#include <atomic>
#include <functional>

#include "lib/string.h"
#include "lib/map.h"
#include "lib/vector.h"

#include "debug/log/Log.h"

namespace ag
{
	struct load_policy {};
	struct async : public load_policy {};

	template <typename T>
	concept load_policy_t = std::is_base_of_v<load_policy, T>;

	using resrc_id_t = uint8_t;

	class Resource
	{

	};

	template <class R>
	concept resource_t = std::is_base_of_v<Resource, R>;

	/**
	* Describes a resource's location
	*/
	template <resource_t T>
	struct resrc_specifier
	{
		friend class resrc_manager;

		resrc_specifier(string fpath)
		{
			path = fpath;
		}

	private:
		string path;
	};

	template <resource_t T>
	struct resrc_handle
	{
		bool is_loaded() const
		{
			return !error && (*status) > 0;
		}

	private:
		/**
		* = 0 if unloaded
		* > 0 otherwise
		*/
		std::atomic<uint8_t>* status;
		bool error;
		T* data;
	};


	class resrc_manager
	{
	public:
		template <load_policy_t P, resource_t T>
		resrc_handle<T> load(resrc_specifier<T> spec, std::function<void(const T&)> callback)
		{
			/// TODO: Policies, async queue, etc
			
			/// TODO: typeids
			constexpr resrcTID = 0;

			auto it = resources.find(spec.path);

			if (it != resources.end())
			{
				if (resources.at(it).typeID != resrcTID)
				{
					Log::Error(sfmt("Loaded resource with incorrect type at {}", spec.path));
					return resrc_handle<T>{.status = nullptr, .error = true, .data = nullptr};
				}

				// Resource present already.
				entry& resrc = resources.at(it);
				/// TODO: callback may get dropped in some cases?
				if (*status > 0)
				{
					callback(*static_cast<T*>(resrc.data));
					return resrc_handle<T>{.status = resrc.status, .error = false, .data = static_cast<T*>(resrc.data)};
				}
			}
		}

	private:
		using resrc_callback = std::function<void(void*)>;
		/// TODO: status ints won't have consistent position depending on map implementation
		struct entry
		{
			std::atomic<uint8_t> status;
			void* data;
			resrc_id_t typeID;
			vector<resrc_callback> waitingCallbacks;
		};
		/// TODO: map not synchronised
		map<string, entry> resources;
	};
}