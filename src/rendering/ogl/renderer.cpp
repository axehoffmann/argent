#include "renderer.h"
#include "renderer.h"

#include "rendering/ogl/opengl.h"
#include "rendering/vertex.h"
#include "rendering/transform.h"

#include "resources/AssetManager.h"
#include "resources/Mesh.h"
#include "resources/Texture.h"

auto loadMesh(const string& path)
{
	u32 rsc = ag::AssetManager::Load<ag::Mesh>(path);
	auto v = ag::AssetManager::Fetch<ag::Mesh>(rsc).lock();
	ag::Log::Trace(ag::sfmt("Loaded mesh with {} indices", v->indices.size()));
	return v;
}


auto loadTex(const string& path)
{
	u32 rsc = ag::AssetManager::Load<ag::Texture>(path);
	auto v = ag::AssetManager::Fetch<ag::Texture>(rsc).lock();
	ag::Log::Trace(ag::sfmt("Loaded tex"));
	return v;
}

renderer::renderer() :
	vbo(buffer_access_type::StaticDraw, buffer_type::VertexData),
	ebo(buffer_access_type::StaticDraw, buffer_type::IndexArray),
	vboOffset(0),
	eboOffset(0),
	vert(),
	s("assets/basic.vs", "assets/basic.fs"),

	instanceData(buffer_access_type::DynamicDraw, buffer_type::Storage),
	textures(buffer_access_type::DynamicDraw, buffer_type::Storage),

	pillar(0),
	cube(1)
{
	// Load 2 models into the VBO + EBO
	auto p = loadMesh("assets/cube.obj");
	auto c = loadMesh("assets/pillar.obj");

	vert.bind();
	vbo.bind();
	ebo.bind();
	vertex::prepareVAO(vert);

	vbo.allocate(sizeof(vertex) * (p->vertices.size() + c->vertices.size()));
	ebo.allocate(sizeof(u32) * (p->indices.size() + c->indices.size()));

	instanceData.allocate(sizeof(render_instance) * 1000);

	glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	auto view = view_matrix({{0, 0, 2}});
	auto proj = projection_matrix(glm::radians(90.0f), 1280.0f / 720.0f, 0.01f, 100.0f);
	s.bind();
	s.uniform("view", view);
	s.uniform("proj", proj);

	s.uniform("lightPos", {1.5, 0, 0});
	s.uniform("viewPos", {0, 0, 2});

	instanceData.bind();
	instanceData.bind(1);

	vert.bind();
}

void renderer::render(scene_graph& scene)
{
	u32 i = 0;
	u32 t = 0;
	for (auto& v : scene.scene)
	{
		instanceData.set(v.data(), sizeof(render_instance) * v.size(), sizeof(render_instance) * i);

		cmdbuf.push({ renderables[t].indexCount, u32(v.size()), renderables[t].firstIndex, renderables[t].baseVertex, i });

		i += v.size();
		t++;
	}
	checkError();

	cmdbuf.bind();
	u32 cmds = cmdbuf.submit();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMultiDrawElementsIndirect(GL_TRIANGLES, static_cast<GLenum>(gltype::U32), (void*)0, cmds, 20);
	checkError();
}

u32 renderer::createRenderable(u32 meshID)
{
	auto m = ag::AssetManager::Fetch<ag::Mesh>(meshID).lock();
	vbo.bind();
	vbo.set(m->vertices.data(), sizeof(vertex) * m->vertices.size(), sizeof(vertex) * vboOffset);

	ebo.bind();
	ebo.set(m->indices.data(), sizeof(u32) * m->indices.size(), sizeof(u32) * eboOffset);

	renderables.push_back({eboOffset, vboOffset, u32(m->indices.size())});
	vboOffset += m->vertices.size();
	eboOffset += m->indices.size();

	return renderables.size() - 1;
}

void renderer::loadMaterial(u32 materialID, u32 texID)
{
	constexpr u32 texCount = 1;

	arr<glhandle, texCount> texs;
	arr<u64, texCount> handles;
	glGenTextures(texCount, texs.data());

	for (u32 i = 0; i < texCount; i++)
	{
		texture t(texs[i]);
		auto r = ag::AssetManager::Fetch<ag::Texture>(texID).lock();
		t.setData(r->width, r->height, r->data);
		handles[i] = t.makeBindless();

		tex.push_back(std::move(t));
	}

	matAllocator.load(gl_material{ handles[0] }, materialID);
}