#pragma once

#include "rendering/transform.h"
#include "rendering/scene_builder.h"

#include "buffer.h"
#include "command_buffer.h"
#include "material_allocator.h"
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

struct point_light
{
	glm::vec4 pos;
	glm::vec4 colour;
};


class renderer
{
public:
	renderer();
	void render(scene_graph& scene);

	u32 createRenderable(u32 meshID);

	void loadMaterial(u32 materialID, arr<u32, 3> texID);

private:

	material_allocator matAllocator;

	command_buffer cmdbuf;

	buffer vbo;
	buffer ebo;
	u32 vboOffset;
	u32 eboOffset;

	buffer instanceData;
	buffer pointLights;

	vector<texture> tex;

	vao vert;
	shader s;

	vector<renderable> renderables;
};