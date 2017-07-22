#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
// layout (location = 3) in vec3 tangent;
// layout (location = 4) in vec3 bitangent;


out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;
// out mat3 tbn;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    TexCoords = aTexCoords;
    WorldPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(model) * aNormal;

		// vec3 t = normalize(vec3(model * vec4(tangent,   0.0)));
		// vec3 b = normalize(vec3(model * vec4(bitangent, 0.0)));
		// vec3 n = normalize(vec3(model * vec4(aNormal,    0.0)));
		// tbn = mat3(t,b,n);

    gl_Position =  projection * view * vec4(WorldPos, 1.0);
}
