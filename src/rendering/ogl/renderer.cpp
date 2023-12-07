#include "renderer.h"

#include "rendering/ogl/opengl.h"
#include "rendering/vertex.h"
#include "rendering/transform.h"

#include "resources/AssetManager.h"
#include "resources/Mesh.h"



auto loadMesh(const string& path)
{
	u32 rsc = ag::AssetManager::Load<ag::Mesh>(path);
	auto v = ag::AssetManager::Fetch<ag::Mesh>(rsc).lock();
	ag::Log::Trace(ag::sfmt("Loaded mesh with {} indices", v->indices.size()));
	return v;
}

renderer::renderer(window& w) :
	win(w),
	model(buffer_access_type::StaticDraw, buffer_type::VertexData),
	ebo(buffer_access_type::StaticDraw, buffer_type::IndexArray),
	vert(),
	s("assets/basic.vs", "assets/basic.fs")
{

	auto p = loadMesh("assets/cube.obj");
	auto c = loadMesh("assets/pillar.obj");

	vert.bind();



	voff =p->vertices.size();
	model.bind();
	model.allocate(sizeof(vertex) * (p->vertices.size() + c->vertices.size()));
	model.set(p->vertices.data(), sizeof(vertex) * voff, 0);
	model.set(c->vertices.data(), sizeof(vertex) * c->vertices.size(), sizeof(vertex) * voff);

	ioff = p->indices.size();
	ebo.bind();
	ebo.setData(p->indices.data(), sizeof(u32) * p->indices.size()); /// TODO: for some reason this is required, otherwise the allocation breaks
	ebo.allocate(sizeof(u32) * (p->indices.size() + c->indices.size()));
	ebo.set(p->indices.data(), sizeof(u32) * ioff, 0);
	ebo.set(c->indices.data(), sizeof(u32) * c->indices.size(), sizeof(u32) * ioff);

	vertex::prepareVAO(vert);

	glClearColor(0.0f, 0.5f, 0.5f, 1.0f);

	t.reserve(400);
	for (u64 i = 0; i < 5; i++)
	{
		for (u64 j = 0; j < 10; j++)
		{
			t.push_back(transform{ {-7.0f + (3.0f * i), -5, -5.0f - (2.5f * j)}, glm::angleAxis(glm::quarter_pi<float>(), glm::vec3{0.0f, 1.0f, -0.5f}), {0.5f,0.5f,0.5f}});
		}
	}

	auto view = view_matrix({{0, 0, 2}});
	auto proj = projection_matrix(glm::radians(90.0f), 1280.0f / 720.0f, 0.01f, 100.0f);
	s.bind();
	s.uniform("view", view);
	s.uniform("proj", proj);
	vert.bind();
}

void renderer::render()
{
	u32 i = 0;
	for (auto& tr : t)
	{
		tr.rot *= glm::quat({0, 0.01, 0.01});
		s.uniform("model["+std::to_string(i) + "]", model_matrix(tr));
		i++;
	}
	checkError();

	cmdbuf.push({36, 25, 0, 0, 0});
	cmdbuf.push({11310, 25, ioff, voff, 25});
	cmdbuf.bind();
	cmdbuf.submit();

	glClear(GL_COLOR_BUFFER_BIT);
	glMultiDrawElementsIndirect(GL_TRIANGLES, static_cast<GLenum>(gltype::U32), (void*)0, 2, 20);

}
