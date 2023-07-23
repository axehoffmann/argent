#pragma once

#include "rendering/RenderEngine.h"
#include "GLScreen.h"
#include "rendering/opengl/structures/GLBuffer.h"
#include "rendering/opengl/structures/GLTexture.h"
#include "rendering/opengl/structures/GLShader.h"
#include "rendering/opengl/structures/GLVAO.h"
#include <unordered_map>

#include "resources/AssetManager.h"
#include "resources/Material.h"
#include "resources/Texture.h"
#include "core/Camera.h"
#include "core/Utility.h"
#include "core/Stats.h"

#include "GL.h"

#include "rendering/opengl/structures/GLShader.h"
#include "rendering/opengl/structures/GLMesh.h"
#include "rendering/opengl/structures/GLTexture.h"
#include "rendering/opengl/structures/GLBuffer.h"



namespace ag
{
	struct GLMaterial
	{
		std::vector<std::shared_ptr<ag::GLTexture>> textures;
		std::vector<float> parameters;
	};

	class GLRenderEngine : public RenderEngine
	{
	public:

		void Render(const ag::SceneGraph& graph) override;

		GLRenderEngine();
		~GLRenderEngine();
	private:

		
		// void InitMesh(uint32_t meshID) override;
		// void InitMaterial(uint32_t materialID) override;
		// std::shared_ptr<ag::GLTexture> InitTexture(uint32_t textureID);
		// 
		// /**
		// * Binds the VBO of the specified mesh
		// * @param meshID The ID of the Mesh resource to bind
		// */
		// void UseMesh(uint32_t meshID);
		// /**
		// * Binds the textures of the specified material, and sets parameters
		// * @param material The ID of the Material resource to bind
		// */
		// void UseMaterial(uint32_t materialID);
		// /**
		// * Binds a texture to the specified slot
		// * @param material The ID of the Texture resource to bind
		// * @param slot The texture slot to bind to
		// */
		// void UseTexture(std::shared_ptr<ag::GLTexture> textureID, int slot);
		// /**
		// * Sets the uniforms for an object's transform
		// * @param tr A pointer to the object's transform
		// * @param camTr A pointer to the camera's transform
		// * @param cam A pointer to the camera data
		// */
		void UseTransform(const ag::Transform& tr, const ag::Transform& camTr, const ag::Camera& cam);

		// std::unordered_map<uint32_t, GLMesh> meshes;
		// std::unordered_map<uint32_t, GLMaterial> materials;
		// std::unordered_map<uint32_t, std::shared_ptr<ag::GLTexture>> textures;

		ag::GLScreen screen;
		ag::GLShader shader;

		ag::GLMesh msh;
	};
}