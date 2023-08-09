#include "Component.h"

std::atomic<ComponentTypeID> ag::ComponentInfo::nextComponentID{0};
std::vector<int> ag::ComponentInfo::componentSize{};

int ag::ComponentInfo::GetSize(ComponentTypeID id)
{
	return componentSize[id];
}