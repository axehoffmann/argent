#include "GLRenderEngine.h"

static std::string SHADER_PATH = "assets/default.shader";
static std::string MESH_PATH = "assets/cube.obj";


void CheckError()
{
	int err = glGetError();
	
		ag::Log::Error(ag::sfmt("OpenGL error: {}", (const char*)glewGetErrorString(err)));
}

ag::GLRenderEngine::GLRenderEngine() : 
	screen(),
	shader(GLShader::FromResource(ag::AssetManager::Load<ag::Shader>(SHADER_PATH))),
	msh(GLMesh::FromResource(ag::AssetManager::Load<ag::Mesh>(MESH_PATH)))
{
	
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
	
}

ag::GLRenderEngine::~GLRenderEngine()
{

}

void ag::GLRenderEngine::Render(const ag::SceneGraph& graph)
{
	shader.Bind();
	msh.Bind();
	shader.Uniform<int>("material.albedo", 0);
	shader.Uniform<int>("material.roughness", 1);
	shader.Uniform<int>("material.metallic", 2);
	shader.Uniform<int>("material.normal", 4);

	shader.Uniform<int>("numPointLights", graph.pointLights.size());
	for (u64 i = 0; const auto& l : graph.pointLights)
	{
		shader.Uniform<glm::vec3>("pointLights[" + std::to_string(i) + "].position", l.position);
		shader.Uniform<glm::vec4>("pointLights[" + std::to_string(i) + "].colour", l.colour);

		i++;
	}

	for (const auto& r : graph.statics)
	{
		if (materials.find(r.materialID) == std::end(materials))
			materials.insert({ r.materialID, GLMaterial(r.materialID) });

		materials.at(r.materialID).Bind({ 0, 1, 2, 4 });

		UseTransform(r.transform, ag::Transform({ 0,1,-3 }, { 0,0,0 }), ag::Camera(90.f, 1280.0f / 720.0f, 0.001f, 10.0f));

	}

	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, msh.indexCount);
	CheckError();

	screen.SwapBuffers();
	screen.PollEvents();
}

void ag::GLRenderEngine::UseTransform(const ag::Transform& tr, const ag::Transform& camTr, const ag::Camera& cam)
{
	glm::mat4 model = ag::Utility::ModelMatrix(tr);
	glm::mat4 view = ag::Utility::ViewMatrix(camTr);
	glm::mat4 projection = ag::Utility::ProjectionMatrix(cam);
	shader.Uniform("model", model, false);
	shader.Uniform("view", view, false);
	shader.Uniform("projection", projection, false);

	//glm::mat4 modelView = ag::Utility::ViewMatrix(camTr) * ag::Utility::ModelMatrix(tr);

	//shader.Uniform<glm::mat4>("modelView", modelView);
	//shader.Uniform<glm::mat4>("mvp", ag::Utility::ProjectionMatrix(cam) * modelView);
}