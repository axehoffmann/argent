#pragma once

#include "resources/v2/resources.h"

#include "lib/vector.h"

#include "zpp_bits.h"

namespace ag
{
	struct material : resource
	{
		using serialize = zpp::bits::members<2>;

		resrc_id shader;
		vector<resrc_id> textures;
	};
}
