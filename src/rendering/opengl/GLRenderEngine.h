#pragma once

#include "../RenderEngine.h"

namespace ag
{
	class GLRenderEngine : public RenderEngine
	{
	public:
		void Initialise() override;

		void Render(ag::SceneGraph* graph) override;

		void LoadMesh(uint32_t meshID) override;
		void UnloadMesh(uint32_t meshID) override;

		void LoadMaterial(uint32_t materialID) override;
		void UnloadMaterial(uint32_t materialID) override;

	private:
		void LoadTexture(uint32_t textureID) override;
	};
}