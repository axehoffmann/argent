#pragma once

#include "rendering/ogl/opengl.h"

#include "shader.h"
#include "texture.h"

class hierarchical_zbuffer
{
public:
	hierarchical_zbuffer(shader&& cs, glhandle depthTex);
	~hierarchical_zbuffer();

	void bind(u32 unit);
	void unbind(u32 unit);

	void generate();

private:
	u32 width;
	u32 height;
	u32 levels;

	shader depthReduce;
	texture depthPyramid;

	glhandle depthSampler; // Used to sample the previous level in the pyramid
	glhandle depthTexture; // The depth texture rendered to when rasterising
};