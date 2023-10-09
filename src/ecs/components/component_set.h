#include "component.h"

namespace ag
{
	template <int N>
	class component_set
	{
	public:
		template <typename ... Ctype>
		constexpr component_set() 
		{
			int idx = 0;
			auto pack = [&idx, &ids](id_t value)
			{
				ids[idx] = value;
				idx++;
			};
			(pack(componentID<Ctype>), ...);
		}

	private:
		id_t ids[N];
	};
}
