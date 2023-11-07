#pragma once

#include <string>
#include <atomic>
#include <functional>

#include "lib/basic.h"
#include "lib/string.h"

namespace ag
{
	using resrc_id_t = u8;

	class Resource
	{
	public:
		virtual i8 load(const string& path) = 0;
	};

	template <class R>
	concept resource_t = std::is_base_of_v<Resource, R>;
}