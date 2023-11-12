#pragma once

#include "GLTexture.h"

#include "lib/basic.h"
#include "lib/vector.h"

namespace ag
{
	class GLMaterial
	{
	public:
		GLMaterial(uint32_t material);

		void Bind(const vector<u8>& slots);

	private:
		vector<GLTexture> textures;
	};
}