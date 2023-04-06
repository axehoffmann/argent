#pragma once

#include <string>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define SCREEN_TITLE "argent"


#define GLEW_STATIC
#include <GLFW/glfw3.h>

namespace ag
{
	class GLScreen
	{
	public:
		void Initialise();
		void SetInfo(std::string info);

	private:
		GLFWwindow* window;
	};
}