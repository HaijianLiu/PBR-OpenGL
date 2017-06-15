// Include standard headers
#include <iostream>
#include <vector>
#include <sys/time.h>
#include <math.h>

// Include header file
#include "opengl.hpp"


// Define settings
#define WINDOW_NAME "PBR"
#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (600)
#define VERTEXSHADER_GLSL   "vertexshader.glsl"
#define FRAGMENTSHADER_GLSL "fragmentshader.glsl"
#define FILE_OBJ            "WPN_MK2Grenade.obj"
#define FILE_DIFFUSE_TGA  	"WPNT_MK2Grenade_Base_Color.tga"
#define FILE_AO_TGA         "WPNT_MK2Grenade_Ambient_occlusion.tga"


// Time Function
struct timeval startTime;
float CurrentTime();

int main(void) {

	// initialize start time
	gettimeofday(&startTime,NULL);

	// Created a OpenGL Window: (Default settings)
	GLFWwindow* window = createWindow(WINDOW_NAME,SCREEN_WIDTH,SCREEN_HEIGHT);

	// Create Vertex Array Object
	GLuint vertexArrayID = getVertexArray();

	// Create and compile our GLSL program from the shaders
	GLuint programID = loadShader(VERTEXSHADER_GLSL,FRAGMENTSHADER_GLSL);

	// Read OBJ file
	GLuint vertexBuffer, uvBuffer, normalBuffer;
	unsigned long count;
	loadObj(FILE_OBJ,vertexBuffer,uvBuffer,normalBuffer,count);

	// Load the texture
	GLuint texDiffuseID = loadTGA(FILE_DIFFUSE_TGA);
	GLuint texAOID = loadTGA(FILE_AO_TGA);

	// Get uniform
	GLuint texDiffuseUniform = glGetUniformLocation(programID,"texDiffuse"); // Get uniform ID
	GLuint texAOUniform = glGetUniformLocation(programID,"texAO"); // Get uniform ID
	GLuint matrixUniform = glGetUniformLocation(programID, "matrixMVP"); // Get uniform ID

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use shaders
		glUseProgram(programID);


		// Projection matrix: 45° Field of View. 4:3 ratio. display range : 0.1 unit <-> 100 units.
		glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(45.0f),(float)SCREEN_WIDTH/(float)SCREEN_HEIGHT,0.1f,100.0f);
		// Camera matrix: Camera is at (4,3,3), in World Space. looks at the origin. Head is up (set to 0,-1,0 to look upside-down).
		glm::mat4 ViewMatrix = glm::lookAt(glm::vec3(4,3,3),glm::vec3(0,1.5,0),glm::vec3(0,1,0));
		// Model matrix : glm::rotate( angle_in_degrees, myRotationAxis )
		glm::mat4 ModelMatrix = glm::rotate(0.5f*CurrentTime(),glm::vec3(0,1,0)) * glm::scale(glm::vec3(0.25)) * glm::mat4(1.0f);
		// Our ModelViewProjection : multiplication of our 3 matrices
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix; // Remember, matrix multiplication is the other way around
		// Send transformation to the currently bound shader,
		// in the "MVP" uniform
		glUniformMatrix4fv(matrixUniform,1,GL_FALSE,&MVP[0][0]);


		// Bind our diffuse texture in Texture Unit 0
		updateTexture(texDiffuseID,texDiffuseUniform,0);
		// Bind our AO texture in Texture Unit 1
		updateTexture(texAOID,texAOUniform,1);

		// Draw model
		updateModel(vertexBuffer,uvBuffer,normalBuffer,count);


		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} while(glfwGetKey(window,GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	// Cleanup VBO
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &uvBuffer);
	glDeleteBuffers(1, &normalBuffer);
	glDeleteTextures(1, &texDiffuseID);
	glDeleteTextures(1, &texAOID);
	glDeleteVertexArrays(1, &vertexArrayID);
	glDeleteProgram(programID);
	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

float CurrentTime() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (float)(tv.tv_sec-startTime.tv_sec)+(tv.tv_usec-startTime.tv_usec)/1000000.0f;
}
