#include "window.h"

window::window()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	screen = glfwCreateWindow(1280, 720, "gfx", nullptr, nullptr);

	if (screen == nullptr)
	{
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(screen);

	glewInit();

	glViewport(0, 0, 1280, 720);
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
