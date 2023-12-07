#pragma once

#include "buffer.h"
#include "command_buffer.h"
#include "shader.h"
#include "vao.h"
#include "window.h"
#include "rendering/transform.h"

class renderer
{
public:
	renderer(window& w);
	void render();

private:
	window& win;

	command_buffer cmdbuf;

	buffer model;
	buffer ebo;

	u32 voff;
	u32 ioff;

	vao vert;
	shader s;

	vector<transform> t;
};