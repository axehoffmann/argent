#include "window.h"

window::window()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	screen = glfwCreateWindow(1920, 1080, "gfx", nullptr, nullptr);

	if (screen == nullptr)
	{
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(screen);

	glewInit();

	if (GLEW_NV_gpu_shader5)
	{
		ag::Log::Trace("aug");
		}
	glViewport(0, 0, 1920, 1080);
}

void window::pollEvents()
{
	glfwPollEvents();
}

void window::swapBuffers()
{
	glfwSwapBuffers(screen);
}

window::~window()
{
	glfwTerminate();
}
