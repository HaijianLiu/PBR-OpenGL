// Use OpenGL 3’s syntax
#version 330 core

// declares the input data
layout(location = 0) in vec3 vertex_position_modelspace;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main(){
  // Output position of the vertex
  gl_Position = MVP * vec4(vertex_position_modelspace,1.0f);
}
