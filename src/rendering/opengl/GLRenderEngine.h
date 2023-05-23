#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "../RenderEngine.h"
#include "GLScreen.h"
#include "GLBuffer.h"
#include "GLTexture.h"
#include <unordered_map>

#include "../../resources/AssetManager.h"
#include "../../resources/Material.h"
#include "../../resources/Texture.h"
#include "../../core/Camera.h"
#include "../../core/Utility.h"
#include "../../core/Stats.h"

#include "GL.h"


namespace ag
{
	struct GLMesh
	{
		ag::GLBuffer* vbo;
		ag::GLBuffer* ebo;

		size_t indexCount;

		GLMesh() { vbo = ebo = nullptr; indexCount = 0; }
		GLMesh(ag::GLBuffer* v, ag::GLBuffer* e, size_t c) : vbo(v), ebo(e), indexCount(c) { }
	};

	struct GLMaterial
	{
		std::vector<std::shared_ptr<ag::GLTexture>> textures;
		std::vector<float> parameters;
	};

	class GLRenderEngine : public RenderEngine
	{
	public:
		void Initialise() override;

		void Render(ag::SceneGraph* graph) override;

		~GLRenderEngine();
	private:

		void InitMesh(uint32_t meshID) override;
		void InitMaterial(uint32_t materialID) override;
		std::shared_ptr<ag::GLTexture> InitTexture(uint32_t textureID);

		/**
		* Binds the VBO of the specified mesh
		* @param meshID The ID of the Mesh resource to bind
		*/
		void UseMesh(uint32_t meshID);
		/**
		* Binds the textures of the specified material, and sets parameters
		* @param material The ID of the Material resource to bind
		*/
		void UseMaterial(uint32_t materialID);
		/**
		* Binds a texture to the specified slot
		* @param material The ID of the Texture resource to bind
		* @param slot The texture slot to bind to
		*/
		void UseTexture(std::shared_ptr<ag::GLTexture> textureID, int slot);
		/**
		* Sets the uniforms for an object's transform
		* @param tr A pointer to the object's transform
		* @param camTr A pointer to the camera's transform
		* @param cam A pointer to the camera data
		*/
		void UseTransform(ag::Transform* tr, ag::Transform* camTr, ag::Camera* cam);

		std::unordered_map<uint32_t, GLMesh> meshes;
		std::unordered_map<uint32_t, GLMaterial> materials;
		std::unordered_map<uint32_t, std::shared_ptr<ag::GLTexture>> textures;

		GLHandle vao;

		ag::GLScreen* screen;

		GLHandle currentShader;
	};
}