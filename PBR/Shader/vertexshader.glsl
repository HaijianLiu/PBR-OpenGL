// Use OpenGL 3’s syntax
#version 330 core

// declares the input data
layout(location = 0) in vec3 vertexPositionModelspace;
layout(location = 1) in vec2 vertexUV;

out vec2 uv;
out vec3 normal;

// Values that stay constant for the whole mesh.
uniform mat4 matrixMVP;

void main(){
  // Output position of the vertex
  gl_Position = matrixMVP * vec4(vertexPositionModelspace,1.0f);
  uv = vertexUV;
}
