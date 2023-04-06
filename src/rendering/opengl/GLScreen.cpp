#include "GLScreen.h"

void ag::GLScreen::Initialise()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE, nullptr, nullptr);

	if (window == nullptr)
	{
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
}

void ag::GLScreen::SetInfo(std::string info)
{
	std::string title = ": " + info;
	title = SCREEN_TITLE + title;
	glfwSetWindowTitle(window, title.c_str());
}
