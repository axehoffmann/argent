#version 460 core

struct GrassBlade
{
	vec4 pos;
};

layout (std430, binding = 12) buffer grassBuf
{
	GrassBlade grassBlades[];
};

out uint bladeHash;
out float posAlongBlade;
out vec3 worldPos;

uniform mat4 view;
uniform mat4 proj;

const float PI = 3.14159;

uniform float clock;

vec2 sampleWind(vec2 pos)
{
	return vec2(1.0, 0.0);
}

// A single iteration of Bob Jenkins' One-At-A-Time hashing algorithm.
uint hash(uint x) 
{
    x += ( x << 10u );
    x ^= ( x >>  6u );
    x += ( x <<  3u );
    x ^= ( x >> 11u );
    x += ( x << 15u );
    return x;
}

uint hash(float xf) 
{
    return hash(floatBitsToUint(xf));
}

uint hashBlade(vec3 v)
{
	return hash(floatBitsToUint(v.x) ^ hash(v.y) ^ hash(v.z));
}

void main() 
{
	GrassBlade blade = grassBlades[gl_InstanceID];
	bladeHash = hashBlade(blade.pos.xyz);
	
	// Randomised Phase Offset 0.0-1.0
	float phaseOffset = float(bladeHash % 25408) / 25408.0 * PI * 5;
	float adjustedClock = clock + phaseOffset;

	// Randomize blade flexibility
	float flex = 0.8 + (bladeHash % 45 / 160.0);

	// Jitter the blade position slightly
	blade.pos.xz += vec2(bladeHash % 50 / 250.0, bladeHash % 33 / 165.0);
	
	// Slightly randomise blade height
	float bladeHeight = 0.9;
	bladeHeight += bladeHash % 19 / 30.0;

	float vertsPerBlade = 15.0;

	posAlongBlade = float(gl_VertexID / 2) / float(vertsPerBlade / 2);
	float vY = posAlongBlade * bladeHeight - (posAlongBlade * (sin(-adjustedClock * PI * 0.2) * 0.25 + 0.4));

	float width = (1.0 - posAlongBlade * posAlongBlade) * 0.1;
	float vX = width * (gl_VertexID % 2) - (width / 2.0);
	vX += sampleWind(blade.pos.xz).x * posAlongBlade * posAlongBlade * (sin(-adjustedClock * PI * 0.2) * 0.1 + 0.5);
	float vZ = (posAlongBlade * posAlongBlade) * (bladeHash % 56 / 270) * 0.5;
	
	worldPos = blade.pos.xyz + vec3(vX, vY, 0.0);
	gl_Position = proj * view * vec4(worldPos, 1.0);
}