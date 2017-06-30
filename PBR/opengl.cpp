// Include standard headers
#include <iostream>

#include "opengl.hpp"

GLFWwindow* createWindow(const char* name, int screenWidth, int screenHeight) {

	// Create OpenGL Window
	GLFWwindow* window;

	// Initialise GLFW
	if (!glfwInit()) {
		printf("[GLFW] Failed to initialize GLFW\n"); // Debug information
		return NULL;
	}

	// Default window settings
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // don't want the old OpenGL

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(screenWidth,screenHeight,name,NULL,NULL);
	if (window == NULL) {
		printf("[GLFW] Failed to open GLFW window.\n"); // Debug information
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		printf("[GLEW] Failed to initialize GLEW\n"); // Debug information
		glfwTerminate();
		return NULL;
	}

	printf("[GLEW] Created a OpenGL Window: %s <%d,%d>\n",name,screenWidth,screenHeight); // Debug information


	// Other Default settings
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// Set input mode GLFW_STICKY_KEYS
	glfwSetInputMode(window,GLFW_STICKY_KEYS,GL_TRUE);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	return window;
}


// Create Vertex Array Object
GLuint getVertexArray() {
	GLuint vertexArrayID;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);
	return vertexArrayID;
}


// Draw model
void updateModel(GLuint vertexBuffer, GLuint uvBuffer, GLuint normalBuffer, unsigned long count) {
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER,uvBuffer);
	glVertexAttribPointer(
		1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER,normalBuffer);
	glVertexAttribPointer(
		2,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES,0,count); // 3 indices starting at 0 -> 1 triangle
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

// Draw model
void updateModel(GLuint vertexBuffer, GLuint uvBuffer, GLuint normalBuffer, GLuint tangentBuffer, GLuint bitangentBuffer, unsigned long count) {
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER,uvBuffer);
	glVertexAttribPointer(
		1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER,normalBuffer);
	glVertexAttribPointer(
		2,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER,tangentBuffer);
	glVertexAttribPointer(
		3,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER,bitangentBuffer);
	glVertexAttribPointer(
		4,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES,0,count); // 3 indices starting at 0 -> 1 triangle
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
}


void updateTexture(GLuint textureID, GLint uniformID, int unit) {
	// Bind our diffuse texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0 + unit); // GL_TEXTURE0
	glBindTexture(GL_TEXTURE_2D,textureID);
	// Set our "DiffuseTextureSampler" sampler to user Texture Unit 0
	glUniform1i(uniformID,unit);
}


// Class Model
Model::Model(const char* path) {
	loadObj(path,vertexBuffer,uvBuffer,normalBuffer,tangentBuffer,bitangentBuffer,count);
}

Model::~Model() {
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &uvBuffer);
	glDeleteBuffers(1, &normalBuffer);
	glDeleteBuffers(1, &tangentBuffer);
	glDeleteBuffers(1, &bitangentBuffer);
}

// Class TexturePBR
TexturePBR::TexturePBR(const char* diffusePath, const char* normalPath, const char* MetalPath, const char* roughPath, const char* aoPath) {
	texDiffuseID = loadTGA(diffusePath);
	texNormalID  = loadTGA(normalPath);
	texMetalID    = loadTGA(MetalPath);
	texRoughID   = loadTGA(roughPath);
	texAOID      = loadTGA(aoPath);
}

TexturePBR::~TexturePBR() {
	glDeleteTextures(1, &texDiffuseID);
	glDeleteTextures(1, &texNormalID);
	glDeleteTextures(1, &texMetalID);
	glDeleteTextures(1, &texAOID);
}


// Rendering Object
void rendering(Object* object, Model* model, TexturePBR* texture, GLuint shader, Camera* camera) {
	// Use shaders
	glUseProgram(shader);

	// Matrix
	glm::mat4 matrixProjection = camera->getMatrixProjection();
	glm::mat4 matrixView       = camera->getMatrixView();
	glm::mat4 matrixModel      = object->getMatrixModel();
	glm::mat4 matrixMVP        = matrixProjection * matrixView * matrixModel;
	// Get uniform
	GLuint matrixMVPUniform   = glGetUniformLocation(shader,"matrixMVP"); // Get uniform ID
	GLuint matrixModelUniform = glGetUniformLocation(shader,"matrixModel"); // Get uniform ID
	// Send transformation to the currently bound shader,
	glUniformMatrix4fv(matrixMVPUniform,1,GL_FALSE,&matrixMVP[0][0]);
	glUniformMatrix4fv(matrixModelUniform,1,GL_FALSE,&matrixModel[0][0]);

	// Texture
	GLuint texDiffuseUniform  = glGetUniformLocation(shader,"texDiffuse"); // Get uniform ID
	GLuint texAOUniform       = glGetUniformLocation(shader,"texAO"); // Get uniform ID
	GLuint texNormalUniform   = glGetUniformLocation(shader,"texNormal"); // Get uniform ID
	GLuint texMetalUniform    = glGetUniformLocation(shader,"texMetal"); // Get uniform ID
	// Bind texture in Texture Unit 0 ~
	updateTexture(texture->texDiffuseID,texDiffuseUniform,0);
	updateTexture(texture->texNormalID,texNormalUniform,1);
	updateTexture(texture->texMetalID,texMetalUniform,2);
	updateTexture(texture->texAOID,texAOUniform,3);

	// Model
	updateModel(model->vertexBuffer,model->uvBuffer,model->normalBuffer,model->tangentBuffer,model->bitangentBuffer,model->count);
}

// Rendering Object
void rendering(Object* object, Model* model, unsigned int texture, GLuint shader, Camera* camera) {
	// Use shaders
	glUseProgram(shader);

	// Matrix
	glm::mat4 matrixProjection = camera->getMatrixProjection();
	glm::mat4 matrixView       = camera->getMatrixView();
	glm::mat4 matrixModel      = object->getMatrixModel();
	glm::mat4 matrixMVP        = matrixProjection * matrixView * matrixModel;
	// Get uniform
	GLuint matrixMVPUniform   = glGetUniformLocation(shader,"matrixMVP"); // Get uniform ID
	GLuint matrixModelUniform = glGetUniformLocation(shader,"matrixModel"); // Get uniform ID
	// Send transformation to the currently bound shader,
	glUniformMatrix4fv(matrixMVPUniform,1,GL_FALSE,&matrixMVP[0][0]);
	glUniformMatrix4fv(matrixModelUniform,1,GL_FALSE,&matrixModel[0][0]);

	// Texture
	GLuint texDiffuseUniform  = glGetUniformLocation(shader,"texDiffuse"); // Get uniform ID
	// Bind texture in Texture Unit 0 ~
	updateTexture(texture,texDiffuseUniform,0);

	// Model
	updateModel(model->vertexBuffer,model->uvBuffer,model->normalBuffer,model->tangentBuffer,model->bitangentBuffer,model->count);
}
