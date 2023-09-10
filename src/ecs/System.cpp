#include "System.h"

void ag::ISystem::SetWorld(std::shared_ptr<ag::World> w)
{
	world = w;
}
