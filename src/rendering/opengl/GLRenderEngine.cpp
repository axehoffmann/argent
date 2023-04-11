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


	std::weak_ptr<ag::Mesh> wMesh = ag::AssetManager::Fetch<ag::Mesh>(meshID);
	
	if (wMesh.expired())
	{
		/// TODO: throw error
		return;
	}

	std::shared_ptr<ag::Mesh> mesh = wMesh.lock();

	GLHandle vbo = ag::GL::MakeBuffer(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Vertex), mesh->vertices.data(), GL_STATIC_DRAW);
	GLHandle ebo = ag::GL::MakeBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(size_t), mesh->indices.data(), GL_STATIC_DRAW);

	meshes[meshID] = GLMesh(vbo, ebo);
}

void ag::GLRenderEngine::InitMaterial(uint32_t materialID)
{
	if (materials.find(materialID) != materials.end())
		return;

	std::weak_ptr<ag::Material> wMaterial = ag::AssetManager::Fetch<ag::Material>(materialID);

	if (wMaterial.expired())
	{
		/// TODO: throw error
		return;
	}

	std::shared_ptr<ag::Material> material = wMaterial.lock();

	ag::GLMaterial glMat;
	glMat.textures.reserve(material->textures.size());

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


	std::weak_ptr<ag::Texture> wTexture = ag::AssetManager::Fetch<ag::Texture>(textureID);

	if (wTexture.expired())
	{
		/// TODO: throw error
		return GLHandle();
	}

	std::shared_ptr<ag::Texture> texture = wTexture.lock();

	GLHandle texHandle = ag::GL::MakeTexture(GL_TEXTURE_2D, GL_RGBA, texture->width, texture->height, texture->data);

	textures[textureID] = texHandle;

	return texHandle;
}

void ag::GLRenderEngine::UseMesh(uint32_t meshID)
{
	glBindBuffer(GL_ARRAY_BUFFER, meshes[meshID].vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[meshID].ebo);
}


void ag::GLRenderEngine::UseMaterial(uint32_t materialID)
{
	for (size_t i = 0; i < materials[materialID].textures.size(); i++)
	{
		UseTexture(materials[materialID].textures[i], i);
	}
}

void ag::GLRenderEngine::UseTexture(uint32_t textureID, int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, textures[textureID]);
}