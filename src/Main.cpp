#include "core/Engine.h"

int main()
{
	ag::Engine e;
	try
	{
		e.Run();
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what();
	}
}