#include "Component.h"

int ag::ComponentInfo::GetSize(ComponentTypeID id)
{
	return componentSize[id];
}