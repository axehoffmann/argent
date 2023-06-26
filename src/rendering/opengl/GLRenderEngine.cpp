#include "GLRenderEngine.h"

void ag::GLRenderEngine::Initialise()
{
	screen = new GLScreen();
	screen->Initialise();
	glewInit();

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.3f, 0.3f, 0.4f, 1.0f);

	vao = new ag::GLBuffer(ag::BufferType::VertexArray, ag::BufferAccessType::StaticDraw);
	shader->InitialiseAttribute("aPos", 3, GL_FLOAT, false, 14 * sizeof(float), 0);
	shader->InitialiseAttribute("aTexCoord", 2, GL_FLOAT, false, 14 * sizeof(float), 3 * sizeof(float));
	shader->InitialiseAttribute("aNormal", 3, GL_FLOAT, false, 14 * sizeof(float), 5 * sizeof(float));
	shader->InitialiseAttribute("aTangent", 3, GL_FLOAT, false, 14 * sizeof(float), 8 * sizeof(float));
	shader->InitialiseAttribute("aBitangent", 3, GL_FLOAT, false, 14 * sizeof(float), 11 * sizeof(float));
}

ag::GLRenderEngine::GLRenderEngine()
{

}
ag::GLRenderEngine::~GLRenderEngine()
{
	for (auto& [id, mesh] : meshes)
	{
		delete mesh.vbo;
		delete mesh.ebo;
	}

	delete screen;
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

	screen->SetInfo(std::to_string(ag::Stats::GetAverageFrameTime()));
	screen->SwapBuffers();
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

std::shared_ptr<ag::GLTexture> ag::GLRenderEngine::InitTexture(uint32_t textureID)
{
	/// TODO: note that data is not unloaded from RAM
	if (textures.find(textureID) != textures.end())
		return textures[textureID];


	std::weak_ptr<ag::Texture> wTexture = ag::AssetManager::Fetch<ag::Texture>(textureID);

	if (wTexture.expired())
	{
		/// TODO: throw error
		return nullptr;
	}

	std::shared_ptr<ag::Texture> texture = wTexture.lock();


	textures[textureID] = std::make_shared<ag::GLTexture>(texture, ag::TextureType::Tex2D, ag::TextureFormat::RGBA);

	return textures[textureID];
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
	shader->Uniform<int>("material.albedo", 0);
	shader->Uniform<int>("material.metallic", 1);
	shader->Uniform<int>("material.roughness", 2);
	shader->Uniform<int>("material.ao", 3);
	shader->Uniform<int>("material.normal", 4);
}

void ag::GLRenderEngine::UseTexture(std::shared_ptr<ag::GLTexture> tex, int slot)
{
	tex->Bind(slot);
}

void ag::GLRenderEngine::UseTransform(ag::Transform* tr, ag::Transform* camTr, ag::Camera* cam)
{
	glm::mat4 modelView = ag::Utility::ViewMatrix(camTr) * ag::Utility::ModelMatrix(tr);

	shader->Uniform<glm::mat4>("modelView", modelView);
	shader->Uniform<glm::mat4>("mvp", ag::Utility::ProjectionMatrix(cam) * modelView);
}