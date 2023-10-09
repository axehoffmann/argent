#include "core/structures/typelist.h"

using ComponentTypes = typelist<>;

namespace ag
{
	template <typename C>
	constexpr id_t componentID = index_of<C, ComponentTypes>();
}