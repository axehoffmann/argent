#version 460 core

in int bladeHash;
in float posAlongBlade;

out vec4 fragColour;

void main()
{
	fragColour = vec4(mix(vec3(0.0, 0.4, 0.7), vec3(0.0, 0.8, 0.4), posAlongBlade), 1.0);
}