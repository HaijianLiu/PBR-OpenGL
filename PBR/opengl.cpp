// Include standard headers
#include <iostream>
#include <vector>
#include <random>

#include "opengl.hpp"

// Include class
#include "Shader.hpp"


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
	glClearColor(0.9,0.9,0.9,1.0);
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


// configure the viewport to the original framebuffer's screen dimensions
void resetViewport(GLFWwindow* window) {
	int scrWidth, scrHeight;
	glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
	glViewport(0, 0, scrWidth, scrHeight);
}

// RenderPass class
RenderPass::RenderPass(GLFWwindow* window, int number) {

	int scrWidth, scrHeight;
	glfwGetFramebufferSize(window, &scrWidth, &scrHeight);

	// configure (floating point) framebuffers
	glGenFramebuffers(1, &this->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

	// create 2 floating point color buffers
	unsigned int colorBuffers[number];
	glGenTextures(number, colorBuffers);
	for (unsigned int i = 0; i < number; i++) {
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, scrWidth, scrHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
		this->pass.push_back(colorBuffers[i]);
	}

	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
	unsigned int attachments[number];
	for (unsigned int i = 0; i < number; i++) {
		attachments[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	glDrawBuffers(number, attachments);

	// create and attach depth buffer (renderbuffer)
	unsigned int depthRBO;
	glGenRenderbuffers(1, &depthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, scrWidth, scrHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);

	// check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	std::cout << "Framebuffer not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER,0);

	// Set Shader
	this->shader = new Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/RenderPassCombine.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/RenderPassCombine.fs.glsl");
	this->shader->use();
	for (unsigned i = 0; i < number; i++) {
		this->shader->setInt(("pass[" + std::to_string(i) + "]").c_str(), i);
	}
}
RenderPass::~RenderPass() {
	delete this->shader;
}


void RenderPass::use() {
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void RenderPass::finish() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void RenderPass::render() {
	this->shader->use();
	for (unsigned int i = 0; i < this->pass.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, this->pass[i]);
	}

	unsigned int quadVAO, quadVBO;
	float quadVertices[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	// setup plane VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}


// generate sample kernel
std::vector<glm::vec3> genSSAOKernel(unsigned int kernelSize) {

	std::vector<glm::vec3> ssaoKernel;

	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
	std::default_random_engine generator;

	for (unsigned int i = 0; i < kernelSize; ++i) {
		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = float(i) / kernelSize;
		// scale samples s.t. they're more aligned to center of kernel
		scale = 0.1f + (1.0f - 0.1f) * scale * scale;
		sample *= scale;
		ssaoKernel.push_back(sample);
	}
	return ssaoKernel;
}

// generate noise texture
unsigned int genNoiseTexture(unsigned int textureSize) {

	std::vector<glm::vec3> ssaoNoise;

	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
	std::default_random_engine generator;

	for (unsigned int i = 0; i < textureSize * textureSize; i++) {
		glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
		ssaoNoise.push_back(noise);
	}
	unsigned int noiseTexture;
	glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, textureSize, textureSize, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return noiseTexture;
}


// // Draw model
// void updateModel(GLuint vertexBuffer, GLuint uvBuffer, GLuint normalBuffer, unsigned long count) {
// 	// 1rst attribute buffer : vertices
// 	glEnableVertexAttribArray(0);
// 	glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer);
// 	glVertexAttribPointer(
// 		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
// 		3,                  // size
// 		GL_FLOAT,           // type
// 		GL_FALSE,           // normalized?
// 		0,                  // stride
// 		(void*)0            // array buffer offset
// 	);
// 	glEnableVertexAttribArray(1);
// 	glBindBuffer(GL_ARRAY_BUFFER,uvBuffer);
// 	glVertexAttribPointer(
// 		1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
// 		2,                  // size
// 		GL_FLOAT,           // type
// 		GL_FALSE,           // normalized?
// 		0,                  // stride
// 		(void*)0            // array buffer offset
// 	);
// 	glEnableVertexAttribArray(2);
// 	glBindBuffer(GL_ARRAY_BUFFER,normalBuffer);
// 	glVertexAttribPointer(
// 		2,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
// 		3,                  // size
// 		GL_FLOAT,           // type
// 		GL_FALSE,           // normalized?
// 		0,                  // stride
// 		(void*)0            // array buffer offset
// 	);
// 	// Draw the triangle !
// 	glDrawArrays(GL_TRIANGLES,0,count); // 3 indices starting at 0 -> 1 triangle
// 	glDisableVertexAttribArray(0);
// 	glDisableVertexAttribArray(1);
// 	glDisableVertexAttribArray(2);
// }
//
// // Draw model
// void updateModel(GLuint vertexBuffer, GLuint uvBuffer, GLuint normalBuffer, GLuint tangentBuffer, GLuint bitangentBuffer, unsigned long count) {
// 	// 1rst attribute buffer : vertices
// 	glEnableVertexAttribArray(0);
// 	glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer);
// 	glVertexAttribPointer(
// 		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
// 		3,                  // size
// 		GL_FLOAT,           // type
// 		GL_FALSE,           // normalized?
// 		0,                  // stride
// 		(void*)0            // array buffer offset
// 	);
// 	glEnableVertexAttribArray(1);
// 	glBindBuffer(GL_ARRAY_BUFFER,uvBuffer);
// 	glVertexAttribPointer(
// 		1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
// 		2,                  // size
// 		GL_FLOAT,           // type
// 		GL_FALSE,           // normalized?
// 		0,                  // stride
// 		(void*)0            // array buffer offset
// 	);
// 	glEnableVertexAttribArray(2);
// 	glBindBuffer(GL_ARRAY_BUFFER,normalBuffer);
// 	glVertexAttribPointer(
// 		2,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
// 		3,                  // size
// 		GL_FLOAT,           // type
// 		GL_FALSE,           // normalized?
// 		0,                  // stride
// 		(void*)0            // array buffer offset
// 	);
// 	glEnableVertexAttribArray(3);
// 	glBindBuffer(GL_ARRAY_BUFFER,tangentBuffer);
// 	glVertexAttribPointer(
// 		3,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
// 		3,                  // size
// 		GL_FLOAT,           // type
// 		GL_FALSE,           // normalized?
// 		0,                  // stride
// 		(void*)0            // array buffer offset
// 	);
// 	glEnableVertexAttribArray(4);
// 	glBindBuffer(GL_ARRAY_BUFFER,bitangentBuffer);
// 	glVertexAttribPointer(
// 		4,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
// 		3,                  // size
// 		GL_FLOAT,           // type
// 		GL_FALSE,           // normalized?
// 		0,                  // stride
// 		(void*)0            // array buffer offset
// 	);
//
// 	// Draw the triangle !
// 	glDrawArrays(GL_TRIANGLES,0,count); // 3 indices starting at 0 -> 1 triangle
// 	glDisableVertexAttribArray(0);
// 	glDisableVertexAttribArray(1);
// 	glDisableVertexAttribArray(2);
// 	glDisableVertexAttribArray(3);
// 	glDisableVertexAttribArray(4);
// }
//
//
// void updateTexture(GLuint textureID, GLint uniformID, int unit) {
// 	// Bind our diffuse texture in Texture Unit 0
// 	glActiveTexture(GL_TEXTURE0 + unit); // GL_TEXTURE0
// 	glBindTexture(GL_TEXTURE_2D,textureID);
// 	// Set our "DiffuseTextureSampler" sampler to user Texture Unit 0
// 	glUniform1i(uniformID,unit);
// }
//
//
// // Class TexturePBR
// TexturePBR::TexturePBR(const char* diffusePath, const char* normalPath, const char* MetalPath, const char* roughPath, const char* aoPath) {
// 	texDiffuseID = loadTGA(diffusePath);
// 	texNormalID  = loadTGA(normalPath);
// 	texMetalID   = loadTGA(MetalPath);
// 	texRoughID   = loadTGA(roughPath);
// 	texAOID      = loadTGA(aoPath);
// }
//
// TexturePBR::~TexturePBR() {
// 	glDeleteTextures(1, &texDiffuseID);
// 	glDeleteTextures(1, &texNormalID);
// 	glDeleteTextures(1, &texMetalID);
// 	glDeleteTextures(1, &texRoughID);
// 	glDeleteTextures(1, &texAOID);
// }
//
//
// // Rendering Object
// void rendering(Object* object, Model* model, TexturePBR* texture, GLuint shader, Camera* camera) {
// 	// Use shaders
// 	glUseProgram(shader);
//
// 	// Matrix
// 	glm::mat4 matrixProjection = camera->getMatrixProjection();
// 	glm::mat4 matrixView       = camera->getMatrixView();
// 	glm::mat4 matrixModel      = object->getMatrixModel();
// 	glm::mat4 matrixMVP        = matrixProjection * matrixView * matrixModel;
// 	// Get uniform
// 	GLuint matrixMVPUniform   = glGetUniformLocation(shader,"matrixMVP"); // Get uniform ID
// 	GLuint matrixModelUniform = glGetUniformLocation(shader,"matrixModel"); // Get uniform ID
// 	// Send transformation to the currently bound shader,
// 	glUniformMatrix4fv(matrixMVPUniform,1,GL_FALSE,&matrixMVP[0][0]);
// 	glUniformMatrix4fv(matrixModelUniform,1,GL_FALSE,&matrixModel[0][0]);
//
// 	// Texture
// 	GLuint texDiffuseUniform  = glGetUniformLocation(shader,"texDiffuse"); // Get uniform ID
// 	GLuint texAOUniform       = glGetUniformLocation(shader,"texAO"); // Get uniform ID
// 	GLuint texNormalUniform   = glGetUniformLocation(shader,"texNormal"); // Get uniform ID
// 	GLuint texMetalUniform    = glGetUniformLocation(shader,"texMetal"); // Get uniform ID
// 	GLuint texRoughUniform    = glGetUniformLocation(shader,"texRough"); // Get uniform ID
// 	// Bind texture in Texture Unit 0 ~
// 	updateTexture(texture->texDiffuseID,texDiffuseUniform,0);
// 	updateTexture(texture->texNormalID,texNormalUniform,1);
// 	updateTexture(texture->texMetalID,texMetalUniform,2);
// 	updateTexture(texture->texRoughID,texRoughUniform,3);
// 	updateTexture(texture->texAOID,texAOUniform,4);
//
// 	// Model
// 	updateModel(model->vertexBuffer,model->uvBuffer,model->normalBuffer,model->tangentBuffer,model->bitangentBuffer,model->count);
// }
//
// // Rendering Object
// void rendering(Object* object, Model* model, unsigned int texture, GLuint shader, Camera* camera) {
// 	// Use shaders
// 	glUseProgram(shader);
//
// 	// Matrix
// 	glm::mat4 matrixProjection = camera->getMatrixProjection();
// 	glm::mat4 matrixView       = camera->getMatrixView();
// 	glm::mat4 matrixModel      = object->getMatrixModel();
// 	glm::mat4 matrixMVP        = matrixProjection * matrixView * matrixModel;
// 	// Get uniform
// 	GLuint matrixMVPUniform   = glGetUniformLocation(shader,"matrixMVP"); // Get uniform ID
// 	GLuint matrixModelUniform = glGetUniformLocation(shader,"matrixModel"); // Get uniform ID
// 	// Send transformation to the currently bound shader,
// 	glUniformMatrix4fv(matrixMVPUniform,1,GL_FALSE,&matrixMVP[0][0]);
// 	glUniformMatrix4fv(matrixModelUniform,1,GL_FALSE,&matrixModel[0][0]);
//
// 	// Texture
// 	GLuint texDiffuseUniform  = glGetUniformLocation(shader,"texDiffuse"); // Get uniform ID
// 	// Bind texture in Texture Unit 0 ~
// 	updateTexture(texture,texDiffuseUniform,0);
//
// 	// Model
// 	updateModel(model->vertexBuffer,model->uvBuffer,model->normalBuffer,model->tangentBuffer,model->bitangentBuffer,model->count);
// }
//
// void renderCamera(Object* object, Model* model, TexturePBR* texture, Shader* shader, Camera* camera) {
//
// 	glDepthFunc(GL_LESS); // set depth function back to default
//
// 	// Use shaders
// 	shader->use();
//
// 	glBindVertexArray(model->vertexArrayID);
//
// 	// Matrix
// 	glm::mat4 matrixProjection = camera->getMatrixProjection();
// 	glm::mat4 matrixView       = camera->getMatrixView();
// 	glm::mat4 matrixModel      = object->getMatrixModel();
// 	glm::mat4 matrixMVP        = matrixProjection * matrixView * matrixModel;
// 	// Get uniform
// 	GLuint matrixMVPUniform   = glGetUniformLocation(shader->programID,"matrixMVP"); // Get uniform ID
// 	GLuint matrixModelUniform = glGetUniformLocation(shader->programID,"matrixModel"); // Get uniform ID
// 	// Send transformation to the currently bound shader,
// 	glUniformMatrix4fv(matrixMVPUniform,1,GL_FALSE,&matrixMVP[0][0]);
// 	glUniformMatrix4fv(matrixModelUniform,1,GL_FALSE,&matrixModel[0][0]);
//
// 	// Texture
// 	GLuint texDiffuseUniform  = glGetUniformLocation(shader->programID,"texDiffuse"); // Get uniform ID
// 	GLuint texAOUniform       = glGetUniformLocation(shader->programID,"texAO"); // Get uniform ID
// 	GLuint texNormalUniform   = glGetUniformLocation(shader->programID,"texNormal"); // Get uniform ID
// 	GLuint texMetalUniform    = glGetUniformLocation(shader->programID,"texMetal"); // Get uniform ID
// 	GLuint texRoughUniform    = glGetUniformLocation(shader->programID,"texRough"); // Get uniform ID
// 	// Bind texture in Texture Unit 0 ~
// 	updateTexture(texture->texDiffuseID,texDiffuseUniform,0);
// 	updateTexture(texture->texNormalID,texNormalUniform,1);
// 	updateTexture(texture->texMetalID,texMetalUniform,2);
// 	updateTexture(texture->texRoughID,texRoughUniform,3);
// 	updateTexture(texture->texAOID,texAOUniform,4);
//
// 	glDrawArrays(GL_TRIANGLES,0,model->count); // 3 indices starting at 0 -> 1 triangle
// 	glBindVertexArray(0);
// 	glDepthFunc(GL_LESS); // set depth function back to default
//
// }
//
// void renderCamera(Object* object, Model* model, TexturePBR* texture, Shader* shader, unsigned int cubemapID, Camera* camera) {
//
// 	glDepthFunc(GL_LESS); // set depth function back to default
//
// 	// Use shaders
// 	shader->use();
//
// 	glBindVertexArray(model->vertexArrayID);
//
// 	// Matrix
// 	glm::mat4 matrixProjection = camera->getMatrixProjection();
// 	glm::mat4 matrixView       = camera->getMatrixView();
// 	glm::mat4 matrixModel      = object->getMatrixModel();
// 	glm::mat4 matrixMVP        = matrixProjection * matrixView * matrixModel;
// 	// Get uniform
// 	GLuint matrixMVPUniform   = glGetUniformLocation(shader->programID,"matrixMVP"); // Get uniform ID
// 	GLuint matrixModelUniform = glGetUniformLocation(shader->programID,"matrixModel"); // Get uniform ID
// 	// Send transformation to the currently bound shader,
// 	glUniformMatrix4fv(matrixMVPUniform,1,GL_FALSE,&matrixMVP[0][0]);
// 	glUniformMatrix4fv(matrixModelUniform,1,GL_FALSE,&matrixModel[0][0]);
//
// 	// Texture
// 	GLuint texDiffuseUniform  = glGetUniformLocation(shader->programID,"texDiffuse"); // Get uniform ID
// 	GLuint texAOUniform       = glGetUniformLocation(shader->programID,"texAO"); // Get uniform ID
// 	GLuint texNormalUniform   = glGetUniformLocation(shader->programID,"texNormal"); // Get uniform ID
// 	GLuint texMetalUniform    = glGetUniformLocation(shader->programID,"texMetal"); // Get uniform ID
// 	GLuint texRoughUniform    = glGetUniformLocation(shader->programID,"texRough"); // Get uniform ID
// 	GLuint texSkyUniform      = glGetUniformLocation(shader->programID,"texSky"); // Get uniform ID
// 	// Bind texture in Texture Unit 0 ~
// 	updateTexture(texture->texDiffuseID,texDiffuseUniform,0);
// 	updateTexture(texture->texNormalID,texNormalUniform,1);
// 	updateTexture(texture->texMetalID,texMetalUniform,2);
// 	updateTexture(texture->texRoughID,texRoughUniform,3);
// 	updateTexture(texture->texAOID,texAOUniform,4);
// 	updateTexture(cubemapID,texSkyUniform,5);
//
// 	glDrawArrays(GL_TRIANGLES,0,model->count); // 3 indices starting at 0 -> 1 triangle
// 	glBindVertexArray(0);
// 	glDepthFunc(GL_LESS); // set depth function back to default
//
// }
