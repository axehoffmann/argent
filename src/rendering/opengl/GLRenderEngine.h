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
		// /**
		// * Sets the uniforms for an object's transform
		// * @param tr A pointer to the object's transform
		// * @param camTr A pointer to the camera's transform
		// * @param cam A pointer to the camera data
		// */
		void UseTransform(const ag::Transform& tr, const ag::Transform& camTr, const ag::Camera& cam);

		ag::GLScreen screen;
		ag::GLShader shader;

		ag::GLTexture tex;
		ag::GLTexture tex2;
		ag::GLTexture tex3;
		ag::GLTexture tex4;

		ag::GLMesh msh;
	};
}