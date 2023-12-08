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

	pillar(0),
	cube(0)
{
	// Load 2 models into the VBO + EBO
	auto p = loadMesh("assets/cube.obj");
	auto c = loadMesh("assets/pillar.obj");

	vert.bind();

	vbo.bind();
	vbo.allocate(sizeof(vertex) * (p->vertices.size() + c->vertices.size()));

	ebo.bind();
	ebo.setData(p->indices.data(), sizeof(u32) * p->indices.size()); /// TODO: for some reason this is required, otherwise the allocation breaks
	ebo.allocate(sizeof(u32) * (p->indices.size() + c->indices.size()));

	cube = createRenderable(ag::AssetManager::Load<ag::Mesh>("assets/cube.obj"));
	pillar = createRenderable(ag::AssetManager::Load<ag::Mesh>("assets/pillar.obj"));

	vertex::prepareVAO(vert);

	auto pt = loadTex("assets/pillar.png");
	tex.setData(pt->width, pt->height, pt->data);
	tex.bind(0);

	glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	// Prepare scene state
	t.reserve(200);
	for (u64 i = 0; i < 25; i++)
	{
		for (u64 j = 0; j < 25; j++)
		{
			t.push_back(transform{ {-15.0f + (3.0f * i), -5, -5.0f - (2.5f * j)}, glm::angleAxis(glm::quarter_pi<float>(), glm::vec3{0.0f, 1.0f, 0.0f}), {0.6f,0.6f,0.6f}});
		}
	}

	auto view = view_matrix({{0, 0, 2}});
	auto proj = projection_matrix(glm::radians(90.0f), 1280.0f / 720.0f, 0.01f, 100.0f);
	s.bind();
	s.uniform("view", view);
	s.uniform("proj", proj);

	s.uniform("diffuse", 0);

	s.uniform("lightPos", {1.5, 0, 0});
	s.uniform("viewPos", {0, 0, 2});

	vert.bind();
}

void renderer::render(const scene_graph& scene)
{
	u32 i = 0;
	for (auto& tr : t)
	{
		tr.rot *= glm::quat({0, 0.01, 0});
		s.uniform("model["+std::to_string(i) + "]", model_matrix(tr));
		i++;
	}
	checkError();

	cmdbuf.push({ renderables[cube].indexCount, 25, 0, 0, 0});
	cmdbuf.push({ renderables[pillar].indexCount, 195, renderables[pillar].firstIndex, renderables[pillar].baseVertex, 25});
	cmdbuf.bind();
	cmdbuf.submit();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMultiDrawElementsIndirect(GL_TRIANGLES, static_cast<GLenum>(gltype::U32), (void*)0, 2, 20);
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
