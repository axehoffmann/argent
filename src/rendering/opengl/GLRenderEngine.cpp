#include "GLRenderEngine.h"

static std::string SHADER_PATH = "assets/default.shader";


ag::GLRenderEngine::GLRenderEngine() : 
	screen(),
	shader(ag::GLShader::FromResource(ag::AssetManager::Load<ag::Shader>(SHADER_PATH))),
	vao(),
	vbo(ag::GLBuffer(ag::BufferType::VertexData, ag::BufferAccessType::StaticDraw))
	
{
	
	// glEnable(GL_DEPTH_TEST);
	glClearColor(0.3f, 0.3f, 0.4f, 1.0f);


	Log::Error(sfmt("GL ERROR: {}", glGetError()));

	vao.Bind();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	vbo.SetData(std::vector<float>{
		-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
	});



	//shader.InitialiseAttribute("aPos", 3, GL_FLOAT, false, 14 * sizeof(float), 0);
	//shader.InitialiseAttribute("aTexCoord", 2, GL_FLOAT, false, 14 * sizeof(float), 3 * sizeof(float));
	//shader.InitialiseAttribute("aNormal", 3, GL_FLOAT, false, 14 * sizeof(float), 5 * sizeof(float));
	//shader.InitialiseAttribute("aTangent", 3, GL_FLOAT, false, 14 * sizeof(float), 8 * sizeof(float));
	//shader.InitialiseAttribute("aBitangent", 3, GL_FLOAT, false, 14 * sizeof(float), 11 * sizeof(float));
	Log::Error(sfmt("GL ERROR: {}", glGetError()));
	
}

ag::GLRenderEngine::~GLRenderEngine()
{

}

void ag::GLRenderEngine::Render(const ag::SceneGraph& graph)
{
	shader.Bind();
	vao.Bind();
	Log::Error(sfmt("GL ERROR: {}", glGetError()));

	glDrawArrays(GL_TRIANGLES, 0, 3);

	/*
	/// TODO: instanced rendering

	for (size_t i = 0; i < graph->statics.size(); i++)
	{
		StaticRenderInstance instance = graph->statics[i];

		UseMaterial(instance.materialID);
		UseMesh(instance.meshID);
		UseTransform(instance.transform, Transform({0, 0, -10}), Camera(90, 1920.0f/1080.0f, 0.01f, 100.0f));

		ag::GL::DrawIndexed(GL_TRIANGLES, meshes[instance.meshID].indexCount, GL_UNSIGNED_INT, 0);
	}
	*/
	screen.SwapBuffers();
	screen.PollEvents();
}
/*
void ag::GLRenderEngine::InitMesh(uint32_t meshID)
{
	/// TODO: note that data is not unloaded from RAM
	if (meshes.find(meshID) != meshes.end())
		return;

	meshes[meshID] = GLMesh::FromResource(meshID);
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
	meshes[meshID].vao.Bind();
}


void ag::GLRenderEngine::UseMaterial(uint32_t materialID)
{
	for (int i = 0; i < materials[materialID].textures.size(); i++)
	{
		UseTexture(materials[materialID].textures[i], i);
	}

	/// TODO: make this agnostic to material format
	shader.Uniform<int>("material.albedo", 0);
	shader.Uniform<int>("material.metallic", 1);
	shader.Uniform<int>("material.roughness", 2);
	shader.Uniform<int>("material.ao", 3);
	shader.Uniform<int>("material.normal", 4);
}

void ag::GLRenderEngine::UseTexture(std::shared_ptr<ag::GLTexture> tex, int slot)
{
	tex->Bind(slot);
}
*/

void ag::GLRenderEngine::UseTransform(const ag::Transform& tr, const ag::Transform& camTr, const ag::Camera& cam)
{
	glm::mat4 modelView = ag::Utility::ViewMatrix(camTr) * ag::Utility::ModelMatrix(tr);

	shader.Uniform<glm::mat4>("modelView", modelView);
	shader.Uniform<glm::mat4>("mvp", ag::Utility::ProjectionMatrix(cam) * modelView);
}