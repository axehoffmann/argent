#include "GLScreen.h"
#include "GLScreen.h"

ag::GLScreen::GLScreen()
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

	glewInit();

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void ag::GLScreen::SetInfo(std::string info)
{
	std::string title = ": " + info;
	title = SCREEN_TITLE + title;
	glfwSetWindowTitle(window, title.c_str());
}

void ag::GLScreen::PollEvents()
{
	glfwPollEvents();
}

void ag::GLScreen::SwapBuffers()
{
	glfwSwapBuffers(window);
}

ag::GLScreen::~GLScreen()
{
	glfwTerminate();
}
