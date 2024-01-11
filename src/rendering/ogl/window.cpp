#include "window.h"

window::window()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	screen = glfwCreateWindow(2560, 1440, "gfx", nullptr, nullptr);

	if (screen == nullptr)
	{
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(screen);

	glewInit();

	if (not GLEW_ARB_bindless_texture)
	{
		ag::Log::Warn("GPU bindless textures not supported!");
	}

	if (not GLEW_NV_gpu_shader5)
	{
		ag::Log::Warn("NV GPU5 extension not supported");
	}

	glViewport(0, 0, 2560, 1440);
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
