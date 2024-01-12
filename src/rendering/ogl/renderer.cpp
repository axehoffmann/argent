#include "renderer.h"
#include "renderer.h"

#include "rendering/ogl/opengl.h"
#include "rendering/transform.h"

#include "resources/AssetManager.h"
#include "resources/Mesh.h"
#include "resources/Texture.h"

#include "grass.h"

void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
{
	std::cout << message << '\n';
}

renderer::renderer(mesh_handler& mh) :
	mesh_info(mh),
	vbo(buffer_access_type::StaticDraw, buffer_type::VertexData),
	ebo(buffer_access_type::StaticDraw, buffer_type::IndexArray),
	vboOffset(0),
	eboOffset(0),
	vert(),
	standardShader("assets/basic.vs", "assets/basic.fs"),
	cullShader("assets/cull.csh"),
	screenShader("assets/screenQuad.vs", "assets/singleTex.fs"),

	instanceData(buffer_access_type::DynamicDraw, buffer_type::Storage),
	pointLights(buffer_access_type::DynamicDraw, buffer_type::Storage),

	sceneInfo(buffer_access_type::DynamicDraw, buffer_type::Storage),
	instanceMap(buffer_access_type::StreamDraw, buffer_type::Storage),

	grassShader("assets/grass.vs", "assets/grass.fs"),
	grassPos(generateGrassBuffer()),
	grassIndices(buffer_access_type::StaticDraw, buffer_type::IndexArray),
	clock(0),

	hzb(shader("assets/buildHzb.csh"), depthLayer.getID())
{

	// Initialise a framebuffer to render to
	colourLayer.allocate(2560, 1440, 1, tex_format::RGBA8);
	fb.attach(colourLayer, attachment::Colour, 0);
	depthLayer.allocate(2560, 1440, 1, tex_format::Depth24);
	fb.attach(depthLayer, attachment::Depth, 0);

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
	sceneInfo.allocate(sizeof(scene_info));
	sceneInfo.bind(10);

	// Some hard-coded lights
	pointLights.allocate(sizeof(u32) + sizeof(point_light) * 256);
	pointLights.bind(4);
	vector<point_light> pls{ 
		{ { 2.5, 0, -2, 0 }, { 0.8, 0.4, 0.2, 30.0 } },
		{ { -4.5, 0, -2, 0 }, { 0.1, 0.4, 1.0, 40.0 } },
		{ { -3.5, 0, -12, 0 }, { 0.4, 0.9, 0.2, 20.0 } },
		{ { 3.5, 0, -12, 0 }, { 0.8, 0.2, 0.9, 20.0 } },
		{ { 0, 70, -50, 0 }, { 0.9, 0.9, 1.0, 35000} },
		{ { 0, -18, -50, 0 }, { 0.2, 0.2, 0.9, 1000} }

	};
	u32 plc = u32(pls.size());
	pointLights.set(&plc, sizeof(u32), 0);
	pointLights.set(pls.data(), sizeof(point_light) * pls.size(), 16);

	// Grass mesh indices
	vector<u32> grs{
		0, 1, 2,	 1, 2, 3,	  2, 3, 4,
		3, 4, 5,	 4, 5, 6,	  5, 6, 7,
		6, 7, 8,	 7, 8, 9,	  8, 9, 10,
		9, 10, 11,	 10, 11, 12,  11, 12, 13,  
		12, 13, 14
	};
	grassIndices.setData(grs.data(), grs.size() * sizeof(u32));

	glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(message_callback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

	auto view = view_matrix({0, 2, 3}, 0, 0.4);
	auto proj = projection_matrix(glm::radians(90.0f), 1280.0f / 720.0f, 0.01f, 200.0f);

	// Prepare shader

	grassShader.bind();
	grassShader.uniform("view", view);
	grassShader.uniform("proj", proj);
	//grassShader.uniform("viewPos", {0, 0, 5});

	vert.bind();
	ebo.bind();
}

f32 clearCol[] { 0.0f, 0.5f, 0.5f };
f32 clearDepth { 1.0f };

void renderer::render(scene_graph& sg, f64 dt)
{
	//glDepthMask(1);
	//hzb.generate();

	glClearNamedFramebufferfv(fb.getID(), GL_COLOR, 0, clearCol);
	glClearNamedFramebufferfv(fb.getID(), GL_DEPTH, 0, &clearDepth);
	glClear(GL_COLOR_BUFFER_BIT);

	/*
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

	// Perform GPU culling and population of the draw commands
	cullShader.bind();
	hzb.bind(0);
	glDispatchCompute(u32(std::ceil(f32(sg.scene.size()) / 256)), 1, 1);
	hzb.unbind(0);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	*/

	glBindFramebuffer(GL_FRAMEBUFFER, fb.getID());
	grassShader.bind();

	clock += dt;

	grassShader.uniform("clock", clock);
	grassIndices.bind();
	grassPos.bind(12);
	glEnable(GL_DEPTH_TEST);
	glDrawElementsInstanced(GL_TRIANGLES, 39, static_cast<GLenum>(gltype::U32), 0, 15 * 15 * 10 * 10);

	// glMultiDrawElementsIndirect(GL_TRIANGLES, static_cast<GLenum>(gltype::U32), (void*)0, cmds, 20);

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

	// Draw framebuffer results to screen (on a full-screen triangle)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	colourLayer.bind(0);
	screenShader.bind();
	screenShader.uniform("screenTex", 0);
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 3);
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