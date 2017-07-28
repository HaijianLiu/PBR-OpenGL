#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;


out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

out vec3 ViewPos;
out vec3 ViewNormal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    TexCoords = aTexCoords;
    WorldPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(model) * aNormal;

		ViewPos = vec3(view * model * vec4(aPos, 1.0));
		mat3 normalMatrix = transpose(inverse(mat3(view * model)));
    ViewNormal = normalMatrix * aNormal;

    gl_Position =  projection * view * vec4(WorldPos, 1.0);
}
