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
	glClearColor(0.9f, 0.9f, 0.9f, 0.9f);
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

void updateTexture(GLuint textureID, GLint uniformID, int unit) {
	// Bind our diffuse texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0 + unit); // GL_TEXTURE0
	glBindTexture(GL_TEXTURE_2D,textureID);
	// Set our "DiffuseTextureSampler" sampler to user Texture Unit 0
	glUniform1i(uniformID,unit);
}
