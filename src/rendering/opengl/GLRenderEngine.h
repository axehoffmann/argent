#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "../RenderEngine.h"
#include "GLScreen.h"
#include <unordered_map>

#include "../../resources/AssetManager.h"
#include "../../resources/Material.h"
#include "../../resources/Texture.h"
#include "../../core/Camera.h"
#include "../../core/Utility.h"


#include "GL.h"


namespace ag
{
	struct GLMesh
	{
		GLHandle vbo;
		GLHandle ebo;

		GLMesh() { vbo = ebo = -1; }
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
		GLHandle InitTexture(uint32_t textureID);

		void UseMesh(uint32_t meshID);
		void UseMaterial(uint32_t materialID);
		void UseTexture(uint32_t textureID, int slot);

		void UseTransform(ag::Transform* tr, ag::Transform* camTr, ag::Camera* cam);

		std::unordered_map<uint32_t, GLMesh> meshes;
		std::unordered_map<uint32_t, GLMaterial> materials;
		std::unordered_map<uint32_t, GLHandle> textures;

		GLHandle vao;

		ag::GLScreen* screen;

		GLHandle currentShader;
	};
}