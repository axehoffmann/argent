#include "hierarchical_zbuffer.h"

hierarchical_zbuffer::hierarchical_zbuffer(shader&& cs) :
	depthReduce(std::move(cs)),
	depthPyramid(),
	levels(u32(std::log2(f32(width))) + 1),

	width(2560),
	height(1440)
{
	checkError();

	// Allocate the HZB texture
	depthPyramid.allocate(width, height, levels, tex_format::Depth);
	checkError();

	// Create a sampler that performs min-reduction instead of linear interpolation
	glCreateSamplers(1, &depthSampler);
	glSamplerParameteri(depthSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(depthSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(depthSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(depthSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	checkError();

	glSamplerParameteri(depthSampler, GL_TEXTURE_REDUCTION_MODE_EXT, GL_MIN);
	checkError();
}

hierarchical_zbuffer::~hierarchical_zbuffer()
{
}

void hierarchical_zbuffer::bindHzb()
{
}

void hierarchical_zbuffer::bindDepthTarget()
{
}

void hierarchical_zbuffer::generate()
{
	depthReduce.bind();
	depthPyramid.bind(1);

	for (u32 level = 0; level < 16; level++)
	{
		glm::vec2 mipSize{ width >> level, height >> level };
		mipSize = glm::max(glm::vec2{ 1, 1 }, mipSize);

		depthReduce.uniform("texSize", mipSize);

		// Sample from the previous mip level
		glSamplerParameteri(depthSampler, GL_TEXTURE_BASE_LEVEL, level - 1);
		glSamplerParameteri(depthSampler, GL_TEXTURE_MAX_LEVEL, level - 1);

		// Write to the next mip level
		glBindImageTexture(1, depthPyramid.getID(), level, false, 0, GL_WRITE_ONLY, static_cast<GLenum>(tex_format::Depth));

		glDispatchCompute(u32(std::ceil(f32(width) / 32)), u32(std::ceil(f32(height) / 32)), 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	}
}
