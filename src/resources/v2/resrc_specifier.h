#pragma once

#include "lib/string.h"
#include "resources.h"

namespace ag
{
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
}