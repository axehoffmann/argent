#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "../RenderEngine.h"
#include "GLScreen.h"
#include <unordered_map>

#include "../../resources/AssetManager.h"

typedef unsigned int GLHandle;

namespace ag
{
	struct GLMesh
	{
		GLHandle vbo;
		GLHandle ebo;

		GLMesh(GLHandle v, GLHandle e) : vbo(v), ebo(e) { }
	};

	struct GLMaterial
	{
		std::vector<GLHandle> textures;
		std::vector<float> parameters;
	};

	class GLRenderEngine : public RenderEngine
	{
	public:
		void Initialise() override;

		void Render(ag::SceneGraph* graph) override;

	private:

		void InitMesh(uint32_t meshID) override;
		void InitMaterial(uint32_t materialID) override;
		void InitTexture(uint32_t textureID) override;

		void UseMesh(uint32_t meshID) override;
		void UseMaterial(uint32_t materialID) override;
		void UseTexture(uint32_t textureID) override;

		std::unordered_map<uint32_t, GLMesh> meshes;
		std::unordered_map<uint32_t, GLMaterial> materials;

		ag::GLScreen* screen;
	};
}