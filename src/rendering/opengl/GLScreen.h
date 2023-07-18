#pragma once

#include "GL.h"
#include <string>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define SCREEN_TITLE "argent"

#include <GLFW/glfw3.h>

namespace ag
{
	class GLScreen
	{
	public:
		void SetInfo(std::string info);
		void SwapBuffers();

		GLScreen();
		~GLScreen();
	private:
		GLFWwindow* window;
	};
}