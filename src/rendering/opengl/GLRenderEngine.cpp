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
	/// TODO: note that data is not unloaded from RAM
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
	if (materials.find(materialID) != materials.end())
		return;

	std::shared_ptr<ag::Material> material = ag::AssetManager::Fetch<ag::Material>(materialID);

	ag::GLMaterial glMat;
	glMat.textures.resize(material->textures.size());

	/// TODO: Copy parameters too

	for (size_t i = 0; i < material->textures.size(); i++)
	{
		glMat.textures[i] = InitTexture(material->textures[i]);
	}

	materials[materialID] = glMat;
}

GLHandle ag::GLRenderEngine::InitTexture(uint32_t textureID)
{
	/// TODO: note that data is not unloaded from RAM
	if (textures.find(textureID) != textures.end())
		return textures[textureID];

	GLHandle texHandle;

	std::shared_ptr<ag::Texture> texture = ag::AssetManager::Fetch<ag::Texture>(textureID);

	glGenTextures(1, &texHandle);
	glBindTexture(GL_TEXTURE_2D, texHandle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->data);

	glGenerateMipmap(GL_TEXTURE_2D);

	textures[textureID] = texHandle;

	return texHandle;
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