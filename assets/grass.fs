#version 460 core

in int bladeHash;
in float posAlongBlade;
in vec3 normal;
in vec3 worldPos;

out vec4 fragColour;

uniform vec3 viewPos;

void main()
{
	// Flip normals on backside of blade
	vec3 N = normal;// * (2 * int(gl_FrontFacing && normal.y > 0) - 1);
	N = abs(normalize(N));

	const vec3 baseColour = vec3(0.0, 0.4, 0.7);
	const vec3 midColour = vec3(0.0, 0.8, 0.4);
	const vec3 tipColour = vec3(0.5, 1.0, 0.4);
	const float m = 0.7;
	const float l = posAlongBlade;

	vec3 colour = mix(mix(baseColour, midColour, l/m), mix(midColour, tipColour, (l - m)/(1.0 - m)), step(m, l)); 
	
	vec3 L = normalize(vec3(0, 1, 0));
	vec3 V = normalize(viewPos - worldPos);
	vec3 H = normalize(L + V);

	vec3 diffuse = max(dot(N, L), 0.0) * colour;
	vec3 spec = pow(max(dot(N, H), 0.0), 32.0) * vec3(1.0, 1.0, 0.9);

	vec3 diffuseTranslucency = vec3(0.3, 0.8, 0.5) * max(dot(-N, L), 0.0);

	fragColour = vec4(diffuse + spec + vec3(0.2) * colour, 1.0);
	//N *= 0.5;
	//N += 0.5;
	//fragColour = vec4(N.x, N.y, N.z, 1.0);
	// fragColour = vec4(posAlongBlade, 0.0, 0.0, 1.0);
}