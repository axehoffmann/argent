#pragma once

#include "rendering/transform.h"
#include "rendering/scene_builder.h"

#include "material_allocator.h"
#include "window.h"
#include "structures/buffer.h"
#include "structures/command_buffer.h"
#include "structures/shader.h"
#include "structures/texture.h"
#include "structures/timer.h"
#include "structures/vao.h"

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

	gltimer cullTimer;

	buffer vbo;
	buffer ebo;
	u32 vboOffset;
	u32 eboOffset;

	buffer instanceData;
	buffer pointLights;

	buffer grassPos;

	buffer sceneInfo;
	command_buffer drawCmds;
	buffer instanceMap;

	vector<texture> tex;

	vao vert;
	shader standardShader;
	shader cullShader;

	vector<renderable> renderables;
};