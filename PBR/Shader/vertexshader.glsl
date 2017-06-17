// Use OpenGL 3’s syntax
#version 330 core

// declares the input data
layout(location = 0) in vec3 vertexModelspace;
layout(location = 1) in vec2 uvModelspace;

out vec2 uv;
out vec3 lightDirectionCameraspace;
// out vec3 texNormalCameraspace;

// Values that stay constant for the whole mesh.
uniform mat4 matrixMVP;
uniform mat4 matrixModel;
uniform mat4 matrixView;

void main(){
  // Output position of the vertex
  gl_Position = matrixMVP * vec4(vertexModelspace,1);
  uv = uvModelspace;


	// // Vector that goes from the vertex to the camera, in camera space.
	// // In camera space, the camera is at the origin (0,0,0).
	vec3 vertexCameraspace = (matrixView * matrixModel * vec4(vertexModelspace,1)).xyz;
	vec3 eyeDirectionCameraspace = vec3(0) - vertexCameraspace;
	// //
	// // // Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 lightCameraspace = (matrixView * vec4(4,3,3,1)).xyz;
	lightDirectionCameraspace = lightCameraspace + eyeDirectionCameraspace;
	//
	// Normal of the the vertex, in camera space
	// texNormalCameraspace = (matrixView * matrixModel * texture(texNormal,uv)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.

}
