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
