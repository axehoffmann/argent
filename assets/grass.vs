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

uniform vec3 viewPos;


vec3 hash3(vec2 p)
{
    vec3 q = vec3(dot(p, vec2(127.1, 311.7)), 
				  dot(p, vec2(269.5, 183.3)), 
				  dot(p, vec2(419.2, 371.9)));

	return fract(sin(q) * 43758.5453);
}

float iqnoise(in vec2 x, float u, float v)
{
    vec2 p = floor(x);
    vec2 f = fract(x);
		
	float k = 1.0+63.0*pow(1.0-v,4.0);
	
	float va = 0.0;
	float wt = 0.0;
    for( int j=-2; j<=2; j++ )
    for( int i=-2; i<=2; i++ )
    {
        vec2 g = vec2( float(i),float(j) );
		vec3 o = hash3( p + g )*vec3(u,u,1.0);
		vec2 r = g - f + o.xy;
		float d = dot(r,r);
		float ww = pow( 1.0-smoothstep(0.0,1.414,sqrt(d)), k );
		va += o.z*ww;
		wt += ww;
    }
	
    return va/wt;
}

vec3 rotateVbyQ(vec3 v, vec4 q)
{
    return 2.0 * dot(v, q.xyz) * q.xyz + (q.w * q.w - dot(q.xyz, q.xyz)) * v + 2.0 * q.w * cross(v, q.xyz);
}

vec2 sampleWind(vec2 pos)
{
	return 2 * iqnoise(pos, 1.0, 1.0) * vec2(0.8, 0.3);
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
	float rot = float(bladeHash % 7978994) / 7978994.0 * PI / 2.0;
	vec3 facing = vec3(cos(rot), 0.0, sin(rot));

	// Randomize blade flexibility
	float flex = 0.7 + (bladeHash % 45 / 100.0);

	// Jitter the blade position slightly
	blade.pos.xz += vec2(bladeHash % 50 / 250.0, bladeHash % 33 / 165.0);
	
	// Slightly randomise blade height
	float bladeHeight = 0.9;
	bladeHeight += bladeHash % 19 / 19.0;

	float vertsPerBlade = 15.0;
	posAlongBlade = float(gl_VertexID / 2) / float(vertsPerBlade / 2);
	
	float width = (1.0 - posAlongBlade * posAlongBlade) * 0.12;

	float horizontalDir = 2.0 * float(gl_VertexID % 2) - 1.0;	
	float horizontal = width * horizontalDir / 2; 
	// Construct the grass blade's base shape
	vec3 basePos = vec3(
		facing.x * horizontal,
		posAlongBlade * bladeHeight,
		facing.z * horizontal
	);

	float horizontalWindSample = sampleWind(blade.pos.xz - vec2(2.0 * clock, 0.0)).x * (sin(-adjustedClock * PI * 2) * 0.1 + 0.5);

	// Bend the blade via wind direction
	vec3 bentPos = basePos + vec3(
		posAlongBlade * posAlongBlade * horizontalWindSample * flex,
		-(posAlongBlade * posAlongBlade * horizontalWindSample * flex),	
		0
	);
	
	// Partial derivatives with respect to posAlongBlade
	float dx = 2 * posAlongBlade * horizontalWindSample * flex;
	float dy = bladeHeight - posAlongBlade * horizontalWindSample * flex;
	
	// A vector tangent to the curve of the blade at posAlongBlade
	vec3 tangent = normalize(vec3(dx, dy, 0.0));
	
	normal = cross(tangent, facing).zyx;
	// Bend the normal outwards slightly for the saddle shape of a grass blade
	// normal = rotateVbyQ(normal, vec4(tangent, horizontalDir * 0.1));

	worldPos = blade.pos.xyz + bentPos;

	// Bend verts slightly in view pos for thicker grass
	float angle = (dot(normal, normalize(viewPos - worldPos)));
	angle = angle * angle;
	float thickenFactor = horizontalDir * angle;
	
	// posAlongBlade = abs(angle);
	gl_Position = proj * view * vec4(worldPos, 1.0);
	
        // VIEWSPACE ADJUSTMENT
        //gl_Position.x += thickenFactor * 0.03;
}
