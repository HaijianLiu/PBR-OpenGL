#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexModelspace;
layout(location = 1) in vec2 uvModelspace;
layout(location = 2) in vec3 normalModelspace;
layout(location = 3) in vec3 tangentModelspace;
layout(location = 4) in vec3 bitangentModelspace;

// Output data ; will be interpolated for each fragment.
out vec2 uv;
out vec3 l;
out vec3 e;
out float lightDistance;

// Values that stay constant for the whole mesh.
uniform mat4 matrixMVP;
uniform mat4 matrixView;
uniform mat4 matrixModel;
uniform mat3 matrixModelView3x3;
uniform vec3 lightPositionWorldspace;

void main(){

	// Output position of the vertexModelspace, in clip space : matrixMVP * position
	gl_Position =  matrixMVP * vec4(vertexModelspace,1);

	// UV of the vertex. No special space for this one.
	uv = uvModelspace;


	// model to camera = ModelView
	vec3 vertexCameraspace = matrixModelView3x3 * vertexModelspace;
	vec3 normalCameraspace = matrixModelView3x3 * normalModelspace;
	vec3 tangentCameraspace = matrixModelView3x3 * tangentModelspace;
	vec3 bitangentCameraspace = matrixModelView3x3 * bitangentModelspace;

	mat3 matrixTBN = transpose(mat3(tangentCameraspace,bitangentCameraspace,normalCameraspace));


	// Vector that goes from the vertexModelspace to the light, in camera space. ModelMatrix is ommited because it's identity.
	vec3 lightPositionCameraspace = (matrixView * vec4(lightPositionWorldspace,1)).xyz;
	vec3 lightDirectionCameraspace = lightPositionCameraspace - vertexCameraspace;
	vec3 lightDirectionTangentspace = matrixTBN * lightDirectionCameraspace;
	// Direction of the light (from the fragment to the light)
	l = normalize(lightDirectionCameraspace);
	// Cosine of the angle between the normalModelspace and the light direction,
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	// Position of the vertexModelspace, in worldspace : ModelMatrix * position
	vec3 vertexWorldspace = (matrixModel * vec4(vertexModelspace,1)).xyz;
	// Distance to the light
	lightDistance = length(lightPositionWorldspace - vertexWorldspace);

	// Vector that goes from the vertexModelspace to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 eyeDirectionCameraspace = vec3(0,0,0) - vertexCameraspace;
	vec3 eyeDirectionTangentspace = matrixTBN * eyeDirectionCameraspace;
	// Eye vector (towards the camera)
	e = normalize(eyeDirectionCameraspace);
	// Direction in which the triangle reflects the light
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1


}
