#include "texture.h"
#include "texture.h"
#include "texture.h"
#include "texture.h"

texture::texture(tex_filter minFilter, tex_type texType, tex_wrap wrap) {
	glCreateTextures(static_cast<GLenum>(texType), 1, &id);

	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(minFilter));
	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(id, GL_TEXTURE_WRAP_S, static_cast<GLenum>(wrap));
	glTextureParameteri(id, GL_TEXTURE_WRAP_T, static_cast<GLenum>(wrap));
}

texture::texture(glhandle glID)
	: id(glID)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &id);

	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);

	f32 aniso;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &aniso);
	glTextureParameterf(id, GL_TEXTURE_MAX_ANISOTROPY, aniso);
}

void texture::allocate(u32 w, u32 h, u32 mips, tex_format fmt)
{
	glTextureStorage2D(id, mips, static_cast<GLenum>(fmt), w, h);
}

void texture::allocate(u32 w, u32 h, tex_format fmt, u32 samples)
{
	glTextureStorage2DMultisample(id, samples, static_cast<GLenum>(fmt), w, h, GL_TRUE);
}

void texture::allocate3D(u32 w, u32 h, u32 d, u32 mips, tex_format fmt)
{
	glTextureStorage3D(id, mips, static_cast<GLenum>(fmt), w, h, d);
}

void texture::setData(u32 w, u32 h, void* data, tex_format fmt)
{
	glTextureStorage2D(id, 1, GL_RGBA8, w, h);
	glTextureSubImage2D(id, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateTextureMipmap(id);
}

void texture::bind(u32 slot)
{
	glBindTextureUnit(slot, id);
}

u64 texture::makeBindless()
{
	u64 blh = glGetTextureHandleARB(id);
	glMakeTextureHandleResidentARB(blh);
	return blh;
}
/*

// E Bruneton's 2017 mapping
void UVtoMuR(vec2 uv, out float mu, out float r)
{
	float H = sqrt(Ra * Ra - Rp * Rp);
	float x_mu = uv.x;
	float x_r = uv.y;
	float rho = H * x_r;
	r = sqrt(rho * rho + Rp * Rp);
	float d_min = Ra - r;
	float d_max = rho + H;
	float d = d_min + x_mu * (d_max - d_min);
	mu = d <= 0.0 ? float(1.0) : (H * H - rho * rho - d * d) / (2.0 * r * d);
	mu = clamp(mu, -1.0, 1.0);
}

// E Bruneton's 2017 mapping
vec2 RMuToUV(float r, float mu)
{
	float H = sqrt(Ra * Ra - Rp * Rp);
	float rho = safeSqrt(r * r - Rp * Rp);
	float d = distToEdgeOfAtmos(r, mu);
	float dMin = Ra - r;
	float dMax = rho + H;
	float muX = (d - dMin) / (dMax - dMin);
	float rX = rho / H;
	return vec2(unitRangeToUV(muX, 256.0), unitRangeToUV(rX, 64.0));
}
*/

