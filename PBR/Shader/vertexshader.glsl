#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexModelspace;
layout(location = 1) in vec2 uvModelspace;
layout(location = 2) in vec3 normalModelspace;
layout(location = 3) in vec3 tangentModelspace;
layout(location = 4) in vec3 bitangentModelspace;

out vec2 uv;

out vec3 vertex;
out vec3 normal;
uniform mat4 matrixMVP;

void main(){

	// Output position of the vertexModelspace, in clip space : matrixMVP * position
	gl_Position =  matrixMVP * vec4(vertexModelspace,1);

	// UV of the vertex. No special space for this one.
	vertex = vertexModelspace;
	uv = uvModelspace;
	normal = normalModelspace;
}
