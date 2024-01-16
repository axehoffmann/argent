#version 460 core

layout (local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

layout (binding = 2, rgba8) uniform writeonly image2D lut;

const float Rp = 6360.0;
const float Ra = 6420.0;

const vec3 rayleighCoeff = vec3(5.47e-6, 1.28e-5, 3.12e-5);
const vec3 mieCoeff		 = vec3(2e-4);

const float PI = 3.14159265359;

// Intersect ray with the atmosphere, returning distance to entry and exit
vec2 raySphere(vec3 P, vec3 dir)
{
	float a = 1.0;
	float b = 2 * dot(P, dir);
	float c = dot(P, P) - Ra * Ra;
	float d = b * b - 4 * a * c;

	if (d > 0) // ray intersected sphere
	{
		float s = sqrt(d);
		float near = max(0, (-b - s) / (2 * a));
		float far = (-b + s) / (2 * a);

		if (far >= 0)
		{
			return vec2(near, far - near);
		}
	}

	// no intersection
	return vec2(1e20, 0);
}

//	h: sample altitude
//	H: scattering scale height (HR ~8000m, HM ~1200m)
float particleDensity(float h, float H)
{
	return exp(-h / H);
}

void main()
{
	vec2 uv = vec2(gl_GlobalInvocationID.xy) / vec2(256, 64);
	
	// Zenith angle - between ray and vertical
	float zenith = uv.x;
	// Altitude above sea level
	float h = uv.y * (Ra - Rp);

	vec3 dir = vec3(sin(zenith * PI / 2.0), cos(zenith * PI / 2.0), 0.0);

	const int samples = 500;

	vec3 dR = vec3(0.0);
	vec3 dM = vec3(0.0);

	vec3 pos = vec3(0.0, Rp + h, 0.0);
	vec2 dsts = raySphere(pos, dir);
	float step = dsts.y / float(samples);

	for (int i = 0; i < samples; i++)
	{
		pos += dir * step;
		float h_i = distance(vec3(0), pos) - Rp;
		
		dR += clamp(particleDensity(h_i, 8.0), 0.0, 1.0) * step;
		dM += clamp(particleDensity(h_i, 1.2), 0.0, 1.0) * step;
	}
	
	dR *= rayleighCoeff;
	dM *= rayleighCoeff;

	vec4 o = vec4(exp(-(dR + dM)), 1.0);

	imageStore(lut, ivec2(gl_GlobalInvocationID.xy), o); 
}
