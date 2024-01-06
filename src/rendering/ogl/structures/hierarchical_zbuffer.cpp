#include "hierarchical_zbuffer.h"

hierarchical_zbuffer::hierarchical_zbuffer(shader&& cs, glhandle depthTex) :
	depthReduce(std::move(cs)),
	depthPyramid(tex_filter::NearestMip),
	levels(u32(std::log2(f32(width))) + 1),
	depthTexture(depthTex),

	width(2560),
	height(1440)
{
	// Allocate the HZB texture
	depthPyramid.allocate(width, height, levels, tex_format::R32);

	// Create a sampler that performs min-reduction instead of linear interpolation
	glCreateSamplers(1, &depthSampler);
	glSamplerParameteri(depthSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(depthSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(depthSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(depthSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glSamplerParameteri(depthSampler, GL_TEXTURE_REDUCTION_MODE_EXT, GL_MIN);
}

hierarchical_zbuffer::~hierarchical_zbuffer()
{
}

void hierarchical_zbuffer::bind(u32 unit)
{
	depthPyramid.bind(unit);
	// glBindSampler(unit, depthSampler);
}

void hierarchical_zbuffer::unbind(u32 unit)
{
	glBindSampler(unit, 0);
	glBindTextureUnit(unit, 0);
}

void hierarchical_zbuffer::generate()
{
	depthReduce.bind();
	depthPyramid.bind(1);

	glBindTextureUnit(0, depthTexture);

	for (u32 level = 0; level < levels; level++)
	{
		glm::vec2 mipSize{ width >> level, height >> level };
		mipSize = glm::max(glm::vec2{ 1, 1 }, mipSize);

		depthReduce.uniform("texSize", mipSize);

		if (level > 0)
		{
			glBindTextureUnit(0, depthPyramid.getID());
			// Sample from the previous mip level
			glSamplerParameterf(depthSampler, GL_TEXTURE_MIN_LOD, level - 1);
			glSamplerParameterf(depthSampler, GL_TEXTURE_MAX_LOD, level - 1);
		}

		// Write to the next mip level
		glBindImageTexture(1, depthPyramid.getID(), level, false, 0, GL_WRITE_ONLY, static_cast<GLenum>(tex_format::R32));
		
		glDispatchCompute(u32(std::ceil(mipSize.x / 32)), u32(std::ceil(mipSize.y / 32)), 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	}
	
	glSamplerParameterf(depthSampler, GL_TEXTURE_MIN_LOD, -100);
	glSamplerParameterf(depthSampler, GL_TEXTURE_MAX_LOD, 100);

	glBindSampler(0, 0);
	glBindTextureUnit(0, 0);
	glBindTextureUnit(1, 0);
}
