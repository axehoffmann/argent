#include "System.h"

std::vector<ag::System*> ag::System::systems{};

void ag::System::Create(std::shared_ptr<ag::World> w)
{
	world = w;
}
