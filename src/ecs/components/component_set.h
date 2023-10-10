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

		constexpr bool operator==(const component_set<N>& other) const
		{
			for (int i = 0; i < N; i++)
			{
				if (ids[i] != other.ids[i])
				{
					return false;
				}
			}
			return true;
		}

	private:
		id_t ids[N];
	};
}
