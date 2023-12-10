#pragma once

#include "rendering/transform.h"
#include "rendering/scene_builder.h"

#include "buffer.h"
#include "command_buffer.h"
#include "shader.h"
#include "texture.h"
#include "vao.h"
#include "window.h"

#include "lib/basic.h"

struct renderable
{
	u32 firstIndex;
	u32 baseVertex;
	u32 indexCount;
};

class renderer
{
public:
	renderer();
	void render(scene_graph& scene);

	u32 createRenderable(u32 meshID);

private:
	command_buffer cmdbuf;

	buffer vbo;
	buffer ebo;
	u32 vboOffset;
	u32 eboOffset;

	buffer instanceData;

	texture tex;

	u32 pillar;
	u32 cube;

	vao vert;
	shader s;

	vector<render_instance> t;

	vector<renderable> renderables;
};