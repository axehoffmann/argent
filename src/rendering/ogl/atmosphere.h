#pragma once

#include "opengl.h"

#include "structures/texture.h"
#include "structures/shader.h"

class atmosphere
{
public:
	atmosphere() :
		transmittanceBuilder("assets/sky/transmittanceLUT.csh"),
		singleScattering("assets/sky/generateLUT.csh"),
		scatterLut(tex_filter::Linear, tex_type::Tex3D, tex_wrap::Clamp),
		tLut(tex_filter::Linear, tex_type::Tex2D, tex_wrap::Clamp)
	{
		tLut.allocate(256, 64, 1, tex_format::RGBA8);
		scatterLut.allocate3D(64, 256, 256, 1, tex_format::RGBA8);
	}

	void runTransmittance()
	{
		transmittanceBuilder.bind();
		glBindImageTexture(2, tLut.getID(), 0, GL_FALSE, 0, GL_WRITE_ONLY, static_cast<GLenum>(tex_format::RGBA8));
		glDispatchCompute(256 / 32, 64 / 32, 1);

		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		
		singleScattering.bind();
		tLut.bind(1);
		glBindImageTexture(0, scatterLut.getID(), 0, GL_FALSE, 0, GL_WRITE_ONLY, static_cast<GLenum>(tex_format::RGBA8));
		glDispatchCompute(64 / 8, 256 / 8, 256 / 8);
		scatterLut.bind(0);
	}

private:
	texture tLut;
	texture scatterLut;

	shader transmittanceBuilder;
	shader singleScattering;

	//shader initialLUT;
	//shader iterateLUT;
};