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
out vec3 normal;

uniform mat4 view;
uniform mat4 proj;

const float PI = 3.14159;
const vec3 unitZ = vec3(0.0, 0.0, 1.0);
const vec3 unitX = vec3(1.0, 0.0, 0.0);

uniform float clock;

vec3 rotateVbyQ(vec3 v, vec4 q)
{
    return v + 2.0 * cross(q.xyz, cross(q.xyz, v) + q.w * v); 
}

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
	
	// Randomised Phase Offset 
	float phaseOffset = float(bladeHash % 25408) / 25408.0 * PI * 4;
	float adjustedClock = clock + phaseOffset;

	// Randomised rotation
	float rot = PI / 4.0;//float(bladeHash % 7978994) / 7978994.0 * PI * 2;
	vec3 facing = vec3(cos(rot), 0.0, sin(rot));

	// Randomize blade flexibility
	// float flex = 0.8 + (bladeHash % 45 / 160.0);

	// Jitter the blade position slightly
	blade.pos.xz += vec2(bladeHash % 50 / 250.0, bladeHash % 33 / 165.0);
	
	// Slightly randomise blade height
	float bladeHeight = 0.9;
	bladeHeight += bladeHash % 19 / 30.0;

	float vertsPerBlade = 15.0;
	posAlongBlade = float(gl_VertexID / 2) / float(vertsPerBlade / 2);
	
	float width = (1.0 - posAlongBlade * posAlongBlade) * 0.1;

	float horizontalDir = 2.0 * float(gl_VertexID % 2) - 1.0;	
	float horizontal = width * horizontalDir / 2; 
	// Construct the grass blade's base shape
	vec3 basePos = vec3(
		facing.x * horizontal,
		posAlongBlade * bladeHeight,
		facing.z * horizontal
	);

	float horizontalWindSample = sampleWind(blade.pos.xz).x * (sin(-adjustedClock * PI * 0.2) * 0.1 + 0.5);

	// Bend the blade via wind direction
	vec3 bentPos = basePos + vec3(
		posAlongBlade * posAlongBlade * horizontalWindSample,
		-(posAlongBlade * posAlongBlade * (sin(-adjustedClock * PI * 0.2) * 0.25 + 0.4)),	
		0
	);
	
	// Partial derivatives with respect to posAlongBlade
	float dx = 2 * posAlongBlade * horizontalWindSample;
	float dy = bladeHeight - posAlongBlade * (sin(-adjustedClock * PI * 0.2) * 0.25 + 0.4);
	
	// A vector tangent to the curve of the blade at posAlongBlade
	vec3 tangent = vec3(dx, dy, 0.0);
	
	normal = cross(tangent, facing).zyx;
	// Bend the normal outwards slightly for the saddle shape of a grass blade
	normal = rotateVbyQ(normal, vec4(normalize(tangent), horizontalDir * 0.2));

	worldPos =  blade.pos.xyz + bentPos;
	gl_Position = proj * view * vec4(worldPos, 1.0);
}