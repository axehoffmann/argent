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
		atmosphereShader("assets/sky/atmosphere.vs", "assets/sky/atmosphere.fs"),
		scatterLut(tex_filter::Linear, tex_type::Tex3D, tex_wrap::Clamp),
		tLut(tex_filter::Linear, tex_type::Tex2D, tex_wrap::Clamp)
	{
		tLut.allocate(256, 64, 1, tex_format::RGBA8);
		scatterLut.allocate3D(128, 512, 512, 1, tex_format::RGBA8);
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
		glDispatchCompute(128 / 8, 512 / 8, 512 / 8);
		

	}

	f32 c = 0;
	void render(f32 clock)
	{
		scatterLut.bind(0);
		c += clock;
		atmosphereShader.bind();
		auto view = view_matrix({ 0, 6370, 0 }, 1.0, -1.8);
		auto proj = projection_matrix(glm::radians(90.0f), 1280.0f / 720.0f, 0.01f, 2000.0f);
		atmosphereShader.uniform("viewInv", glm::inverse(view));
		atmosphereShader.uniform("projInv", glm::inverse(proj));
		atmosphereShader.uniform("camPos", { 0, 6370, 0 });
		atmosphereShader.uniform("sunDirection", glm::normalize(glm::axis(glm::quat({ clock, glm::half_pi<float>(), 0}))));
		glDisable(GL_DEPTH_TEST);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

private:
	texture tLut;
	texture scatterLut;

	shader transmittanceBuilder;
	shader singleScattering;
	shader atmosphereShader;
	//shader initialLUT;
	//shader iterateLUT;
};