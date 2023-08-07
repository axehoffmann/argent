#include "core/Engine.h"

int main()
{
	std::unique_ptr<ag::Engine> e = std::make_unique<ag::Engine>();
	e->Run();
}