#include "GLRenderEngine.h"

void ag::GLRenderEngine::Initialise()
{
	screen = new GLScreen();
	screen->Initialise();

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.3f, 0.3f, 0.4f, 1.0f);

	vao = ag::GL::MakeVAO();
	ag::GL::InitialiseAttribute("aPos", 3, GL_FLOAT, false, 14 * sizeof(float), 0, currentShader);
	ag::GL::InitialiseAttribute("aTexCoord", 2, GL_FLOAT, false, 14 * sizeof(float), 3 * sizeof(float), currentShader);
	ag::GL::InitialiseAttribute("aNormal", 3, GL_FLOAT, false, 14 * sizeof(float), 5 * sizeof(float), currentShader);
	ag::GL::InitialiseAttribute("aTangent", 3, GL_FLOAT, false, 14 * sizeof(float), 8 * sizeof(float), currentShader);
	ag::GL::InitialiseAttribute("aBitangent", 3, GL_FLOAT, false, 14 * sizeof(float), 11 * sizeof(float), currentShader);

}

ag::GLRenderEngine::~GLRenderEngine()
{

}

void ag::GLRenderEngine::Render(ag::SceneGraph* graph)
{
	/// TODO: instanced rendering

	for (size_t i = 0; i < graph->statics.size(); i++)
	{
		StaticRenderInstance instance = graph->statics[i];

		UseMaterial(instance.materialID);
		UseMesh(instance.meshID);
		UseTransform(&instance.transform, new Transform({0, 0, -10}), new Camera(90, 1920.0f/1080.0f, 0.01f, 100.0f));

		ag::GL::DrawIndexed(GL_TRIANGLES, meshes[instance.meshID].indexCount, GL_UNSIGNED_INT, 0);
	}
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

	ag::GLBuffer* vbo = new ag::GLBuffer(ag::BufferType::VertexArray, ag::BufferAccessType::StaticDraw);
	vbo->SetData(mesh->vertices);
	ag::GLBuffer* ebo = new ag::GLBuffer(ag::BufferType::IndexArray, ag::BufferAccessType::StaticDraw);
	ebo->SetData(mesh->indices);

	meshes[meshID] = GLMesh(vbo, ebo, mesh->indices.size());
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
	meshes[meshID].vbo->Bind();
	meshes[meshID].ebo->Bind();
}


void ag::GLRenderEngine::UseMaterial(uint32_t materialID)
{
	for (size_t i = 0; i < materials[materialID].textures.size(); i++)
	{
		UseTexture(materials[materialID].textures[i], i);
	}

	/// TODO: make this agnostic to material format
	ag::GL::SetInt("material.albedo", 0, currentShader);
	ag::GL::SetInt("material.metallic", 1, currentShader);
	ag::GL::SetInt("material.roughness", 2, currentShader);
	ag::GL::SetInt("material.ao", 3, currentShader);
	ag::GL::SetInt("material.normal", 4, currentShader);

}

void ag::GLRenderEngine::UseTexture(uint32_t textureID, int slot)
{
	ag::GL::BindTexture(GL_TEXTURE_2D, slot, textures[textureID]);
}

void ag::GLRenderEngine::UseTransform(ag::Transform* tr, ag::Transform* camTr, ag::Camera* cam)
{
	glm::mat4 modelView = ag::Utility::ViewMatrix(camTr) * ag::Utility::ModelMatrix(tr);

	ag::GL::SetMat4("modelView", modelView, currentShader);
	ag::GL::SetMat4("mvp", ag::Utility::ProjectionMatrix(cam) * modelView, currentShader);
}
