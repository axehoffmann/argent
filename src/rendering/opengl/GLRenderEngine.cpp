#include "GLRenderEngine.h"

void ag::GLRenderEngine::Initialise()
{
	screen = new GLScreen();
	screen->Initialise();

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.3f, 0.3f, 0.4f, 1.0f);


}

void ag::GLRenderEngine::Render(ag::SceneGraph* graph)
{

}

void ag::GLRenderEngine::InitMesh(uint32_t meshID)
{
	if (meshes.find(meshID) != meshes.end())
		return;

	std::shared_ptr<ag::Mesh> mesh = ag::AssetManager::Fetch<ag::Mesh>(meshID);
	GLHandle vbo, ebo;
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Vertex), mesh->vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Vertex), mesh->indices.data(), GL_STATIC_DRAW);

	meshes.emplace(meshID, GLMesh(vbo, ebo));
}

void ag::GLRenderEngine::InitMaterial(uint32_t materialID)
{

}

void ag::GLRenderEngine::InitTexture(uint32_t textureID)
{
}

void ag::GLRenderEngine::UseMesh(uint32_t meshID)
{
}


void ag::GLRenderEngine::UseMaterial(uint32_t materialID)
{
}

void ag::GLRenderEngine::UseTexture(uint32_t textureID)
{
}