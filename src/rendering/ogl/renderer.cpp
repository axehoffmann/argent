#include "renderer.h"
#include "renderer.h"

#include "rendering/ogl/opengl.h"
#include "rendering/transform.h"

#include "resources/AssetManager.h"
#include "resources/Mesh.h"
#include "resources/Texture.h"

#include "grass.h"

renderer::renderer(mesh_handler& mh) :
	mesh_info(mh),
	vbo(buffer_access_type::StaticDraw, buffer_type::VertexData),
	ebo(buffer_access_type::StaticDraw, buffer_type::IndexArray),
	vboOffset(0),
	eboOffset(0),
	vert(),
	standardShader("assets/basic.vs", "assets/basic.fs"),
	cullShader("assets/cull.csh"),

	instanceData(buffer_access_type::DynamicDraw, buffer_type::Storage),
	pointLights(buffer_access_type::DynamicDraw, buffer_type::Storage),
	grassPos(generateGrassBuffer()),

	sceneInfo(buffer_access_type::DynamicDraw, buffer_type::Storage),
	instanceMap(buffer_access_type::StreamDraw, buffer_type::Storage),

	cullTimer([](f32 ms) {
		ag::Log::Trace(ag::sfmt("cull time: {}ms", ms));
	})
{
	// Initialise mesh buffers
	vert.bind();
	prepareVAOStandard(vert, vbo, ebo);
	vbo.allocate(sizeof(basic_vertex)  * 1000000);
	ebo.allocate(sizeof(u32)	* 1000000);

	// Initialise instance storage
	instanceData.allocate(sizeof(instance_data) * 1e6);
	instanceData.bind(1);
	instanceMap.allocate(sizeof(u32) * 1e6);
	instanceMap.bind(11);

	// Auxiliary buffers for GPU-driven rendering
	drawCmds.bind();
	drawCmds.bind(12);
	checkError();
	sceneInfo.allocate(sizeof(scene_info));
	sceneInfo.bind(10);
	checkError();

	// Some hard-coded lights
	pointLights.allocate(sizeof(u32) + sizeof(point_light) * 256);
	pointLights.bind(4);
	checkError();
	vector<point_light> pls{ 
		{ { 2.5, 0, -2, 0 }, { 0.8, 0.4, 0.2, 30.0 } },
		{ { -4.5, 0, -2, 0 }, { 0.1, 0.4, 1.0, 40.0 } },
		{ { -3.5, 0, -12, 0 }, { 0.4, 0.9, 0.2, 20.0 } },
		{ { 3.5, 0, -12, 0 }, { 0.8, 0.2, 0.9, 20.0 } }
	};
	u32 plc = u32(pls.size());
	pointLights.set(&plc, sizeof(u32), 0);
	pointLights.set(pls.data(), sizeof(point_light) * pls.size(), 16);

	glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);

	auto view = view_matrix({0, 0, 2}, 0, 0);
	auto proj = projection_matrix(glm::radians(90.0f), 1280.0f / 720.0f, 0.01f, 200.0f);

	// Prepare shader
	standardShader.bind();
	standardShader.uniform("view", view);
	standardShader.uniform("proj", proj);
	standardShader.uniform("viewPos", {0, 0, 5});

	vert.bind();
}

void renderer::render(scene_graph& sg)
{
	checkError();
	glDepthMask(1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Update some scene data
	instanceData.set(sg.scene.data(), sizeof(instance_data) * sg.scene.size(), 0);
	sceneInfo.set(&sg.info, sizeof(scene_info), 0);

	// Generate base draw commands
	u32 baseInstance = 0;
	for (u32 i = 0; i < sg.meshCounts.size(); i++)
	{
		renderable& r = renderables.at(i);

		drawCmds.push({ r.indexCount, 0, r.firstIndex, r.baseVertex, baseInstance });
		baseInstance += sg.meshCounts[i];
	}
	u32 cmds = drawCmds.submit();

	cullTimer.start();
	// Perform GPU culling and population of the draw commands
	cullShader.bind();
	glDispatchCompute(u32(std::ceil(f32(sg.scene.size()) / 256)), 1, 1);
	glMemoryBarrier(GL_BUFFER_UPDATE_BARRIER_BIT);
	cullTimer.stop();
	
	standardShader.bind();
	/*
	// Depth prepass
	glColorMask(0, 0, 0, 0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glMultiDrawElementsIndirect(GL_TRIANGLES, static_cast<GLenum>(gltype::U32), (void*)0, cmds, 20);
	
	// Main draw
	glDepthMask(1);
	glColorMask(1, 1, 1, 1);
	glDepthFunc(GL_EQUAL);
	*/
	glMultiDrawElementsIndirect(GL_TRIANGLES, static_cast<GLenum>(gltype::U32), (void*)0, cmds, 20);
}

u32 renderer::createRenderable(u32 meshID)
{
	auto m = ag::AssetManager::Fetch<ag::Mesh>(meshID).lock();
	vbo.bind();
	vbo.set(m->mesh.vertices.data(), sizeof(basic_vertex) * m->mesh.vertices.size(), sizeof(basic_vertex) * vboOffset);

	ebo.bind();
	ebo.set(m->mesh.indices.data(), sizeof(u32) * m->mesh.indices.size(), sizeof(u32) * eboOffset);

	renderables.push_back({eboOffset, vboOffset, u32(m->mesh.indices.size())});
	vboOffset += u32(m->mesh.vertices.size());
	eboOffset += u32(m->mesh.indices.size());

	mesh_info.baseBoundingSpheres.push_back(m->mesh.boundingSphere);

	return u32(renderables.size()) - 1;
}

void renderer::loadMaterial(u32 materialID, arr<u32, 3> texID)
{
	arr<glhandle, 3> texs;
	arr<u64, 3> handles;
	glGenTextures(3, texs.data());

	for (u32 i = 0; i < 3; i++)
	{
		texture t(texs[i]);
		auto r = ag::AssetManager::Fetch<ag::Texture>(texID[i]).lock();
		t.setData(r->width, r->height, r->data);
		handles[i] = t.makeBindless();

		tex.push_back(std::move(t));
	}

	matAllocator.load(gl_material{ handles[0], handles[1], handles[2] }, materialID);
}