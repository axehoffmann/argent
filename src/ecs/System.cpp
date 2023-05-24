#include "System.h"

std::vector<ag::System::factory_func*> ag::System::systems{};

void ag::System::Create(std::shared_ptr<ag::World> w)
{
	world = w;
}
