#pragma once

#include "buffer.h"
#include "vao.h"
#include "window.h"

class renderer
{
public:
	renderer(window& w);
	void render();

private:
	window& win;

	buffer model;
	vao vert;
};