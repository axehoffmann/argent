#pragma once

#include "opengl.h"

#include "structures/texture.h"
#include "structures/shader.h"

class atmosphere
{
public:
	atmosphere() :
		transmittanceBuilder("assets/sky/transmittanceLUT.csh")
	{
		lut.allocate(256, 64, 1, tex_format::RGBA8);
	}

	void runTransmittance()
	{
		transmittanceBuilder.bind();
		glBindImageTexture(2, lut.getID(), 0, GL_FALSE, 0, GL_WRITE_ONLY, static_cast<GLenum>(tex_format::RGBA8));
		glDispatchCompute(256 / 32, 64 / 32, 1);
		lut.bind(0);
	}

private:
	texture lut;
	shader transmittanceBuilder;

	//shader initialLUT;
	//shader iterateLUT;
};