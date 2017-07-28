#version 330 core
layout (location = 0) out vec4 albedoColor;
layout (location = 4) out vec4 alpha;
in vec3 WorldPos;

uniform samplerCube environmentMap;

void main()
{
	vec3 envColor = textureLod(environmentMap, WorldPos, 0.0).rgb;
	// vec3 envColor = pow(textureLod(environmentMap, WorldPos, 0.0).rgb,vec3(2.2));

    // HDR tonemap and gamma correct
    // envColor = envColor / (envColor + vec3(1.0));
    // envColor = pow(envColor, vec3(1.0/2.2));

	albedoColor = vec4(envColor, 1.0);
	alpha = vec4(0.0,0.0,0.0,1.0);
}
