#pragma once

#include "rendering/ogl/opengl.h"

#include "shader.h"
#include "texture.h"

class hierarchical_zbuffer
{
public:
	hierarchical_zbuffer(shader&& cs);
	~hierarchical_zbuffer();

	void bindHzb();
	void bindDepthTarget();
	void generate();

private:
	u32 width;
	u32 height;
	u32 levels;

	shader depthReduce;
	texture depthPyramid;

	glhandle depthSampler; // Used to sample the previous level in the pyramid
	glhandle depthImage; // Used to write to a mip level
};