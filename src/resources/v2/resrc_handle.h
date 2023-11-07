#pragma once

#include "lib/basic.h"
#include "lib/sync.h"

#include "resources.h"

namespace ag
{
	template <resource_t T>
	struct resrc_handle
	{
		bool is_loaded() const
		{
			return (*status) > 0;
		}

		template <typename Func>
		bool use(Func f)
		{
			if (!is_loaded())
				return false;

			f(*data);

			return true;
		}

	private:
		friend class resrc_manager;

		resrc_handle(atomic<i8>* status_p, T* data_p) : status(status_p), data(data_p) {}

		/**
		* = 0 if unloaded
		* > 0 otherwise
		*/
		atomic<i8>* status;
		T* data;
	};
}