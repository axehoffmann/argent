#include "ECSTypes.h"
#include "ArchetypeCollection.h"

namespace ag
{
	// An entity doesn't actually store any data, it just indexes into an ArchetypeCollection.
	class Entity
	{
	public:
		Entity(EntityID id);

		template <typename C>
		C* Get()
		{
			size_t index = archetype->GetIndexByID(ID);
			if (index == -1)
				return nullptr;
			return archetype->GetComponent<C>(index);
		}

		EntityID GetID();

	private:
		EntityID ID;
		ArchetypeCollection* archetype;
	};
}