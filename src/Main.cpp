#include "lib/basic.h"

#include "core/Engine.h"

#include "rendering/ogl/window.h"
#include "rendering/ogl/renderer.h"

#include "debug/log/Log.h"

i32 main()
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