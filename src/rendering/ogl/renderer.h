#pragma once

#include "rendering/scene_builder.h"
#include "rendering/transform.h"

#include "material_allocator.h"
#include "window.h"

#include "structures/buffer.h"
#include "structures/command_buffer.h"
#include "structures/framebuffer.h"
#include "structures/hierarchical_zbuffer.h"
#include "structures/shader.h"
#include "structures/texture.h"
#include "structures/timer.h"
#include "structures/vao.h"

#include "atmosphere.h"

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
	renderer(mesh_handler& mh);
	void render(scene_graph& scene, f64 dt);

	u32 createRenderable(u32 meshID);

	void loadMaterial(u32 materialID, arr<u32, 3> texID);

private:
	mesh_handler& mesh_info;

	material_allocator matAllocator;

	framebuffer fb;
	texture colourLayer;
	texture depthLayer;
	hierarchical_zbuffer hzb;

	buffer vbo;
	buffer ebo;
	u32 vboOffset;
	u32 eboOffset;

	f32 clock;

	buffer instanceData;
	buffer pointLights;

	buffer grassPos;
	buffer grassIndices;

	buffer sceneInfo;
	command_buffer drawCmds;
	buffer instanceMap;

	vector<texture> tex;

	vao vert;
	shader standardShader;
	shader cullShader;
	shader screenShader;
	shader grassShader;

	atmosphere atmo;

	vector<renderable> renderables;
};