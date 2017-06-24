#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexModelspace;
layout(location = 1) in vec2 uvModelspace;
layout(location = 2) in vec3 normalModelspace;
layout(location = 3) in vec3 tangentModelspace;
layout(location = 4) in vec3 bitangentModelspace;


out vec3 vertex;
out vec2 uv;
out vec3 normal;
out mat3 tbn;

uniform mat4 matrixMVP;
uniform mat4 matrixModel;

void main(){

	// Output position of the vertexModelspace, in clip space : matrixMVP * position
	gl_Position =  matrixMVP * vec4(vertexModelspace,1);

	// UV of the vertex. No special space for this one.
	vertex = vertexModelspace;
	uv = uvModelspace;
	normal = normalModelspace;

	vec3 t = normalize(vec3(matrixModel * vec4(tangentModelspace,   0.0)));
	vec3 b = normalize(vec3(matrixModel * vec4(bitangentModelspace, 0.0)));
	vec3 n = normalize(vec3(matrixModel * vec4(normalModelspace,    0.0)));
	tbn = mat3(t,b,n);
}
