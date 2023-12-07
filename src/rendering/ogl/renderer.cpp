#include "renderer.h"

#include "rendering/ogl/opengl.h"
#include "rendering/vertex.h"

#include "resources/AssetManager.h"
#include "resources/Mesh.h"

void checkError(std::source_location loc = std::source_location::current())
{
	int err = glGetError();
	if (err)
		ag::Log::Error(ag::sfmt("OpenGL error: [{}] {}", err, (const char*)glewGetErrorString(err)), loc);
}

renderer::renderer(window& w) :
	win(w),
	model(buffer_access_type::StaticDraw, buffer_type::VertexData),
	vert(),
	s("assets/basic.vs", "assets/basic.fs")
{
	u32 rsc = ag::AssetManager::Load<ag::Mesh>("assets/cube.obj");
	auto v = ag::AssetManager::Fetch<ag::Mesh>(rsc).lock();
	
	ag::Log::Trace(ag::sfmt("Loaded mesh with {} verts", v->vertices.size()));

	model.bind();
	model.setData(v->vertices.data(), sizeof(vertex) * v->vertices.size());

	vertex::prepareVAO(vert);
	glClearColor(0.0f, 0.5f, 0.5f, 1.0f);

	checkError();
}

void renderer::render()
{
	vert.bind();
	s.bind();

	auto mat = glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, -1.0f});
	mat = glm::rotate(mat, glm::quarter_pi<float>(), {1.0f, 1.0f, 0.5f});

	auto view = glm::lookAt(glm::vec3{0.0f, 0.0f, 2.0f }, {0.0f, 0.0f, 1.0f }, {0.0f, 1.0f, 0.0f});
	auto proj = glm::perspective(glm::radians(100.0f), 1280.0f / 720.0f, 0.01f, 100.0f);

	s.uniform("model", mat);
	s.uniform("view", view);
	s.uniform("proj", proj);
	checkError();

	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 36);

}
