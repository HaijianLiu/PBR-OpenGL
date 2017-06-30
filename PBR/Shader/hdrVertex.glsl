#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexModelspace;

out vec3 vertex;

uniform mat4 matrixMVP;

void main(){

	// Output position of the vertexModelspace, in clip space : matrixMVP * position
	gl_Position =  matrixMVP * vec4(vertexModelspace,1);

	// UV of the vertex. No special space for this one.
	vertex = vertexModelspace;
}
