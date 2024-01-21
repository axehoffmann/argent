#pragma once

#include "opengl.h"
#include <string>

#include <GLFW/glfw3.h>


class window
{
public:
	inline static GLFWwindow* windo = nullptr;

	void pollEvents();
	void swapBuffers();

	window();
	~window();
private:
	GLFWwindow* screen;
};