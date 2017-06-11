// Use OpenGL 3’s syntax
#version 330 core

// declares the input data
layout(location = 0) in vec3 vertex_position_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;

out vec2 UV;
out vec3 normal;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main(){
  // Output position of the vertex
  gl_Position = MVP * vec4(vertex_position_modelspace,1.0f);
  UV = vertexUV;
  normal = vertexNormal;
}
