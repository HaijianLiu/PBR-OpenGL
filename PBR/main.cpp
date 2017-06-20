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
#define FILE_NORMAL_TGA     "WPNT_MK2Grenade_Normal_DirectX.tga"
#define FILE_SPEC_TGA       "WPNT_MK2Grenade_Metallic.tga"


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
	GLuint vertexBuffer, uvBuffer, normalBuffer, tangentBuffer, bitangentBuffer;
	unsigned long count;
	loadObj(FILE_OBJ,vertexBuffer,uvBuffer,normalBuffer,tangentBuffer,bitangentBuffer,count);

	// Load the texture
	GLuint texDiffuseID = loadTGA(FILE_DIFFUSE_TGA);
	GLuint texAOID = loadTGA(FILE_AO_TGA);
	GLuint texNormalID = loadTGA(FILE_NORMAL_TGA);
	GLuint texSpecID = loadTGA(FILE_SPEC_TGA);

	// Get uniform
	GLuint texDiffuseUniform = glGetUniformLocation(programID,"texDiffuse"); // Get uniform ID
	GLuint texAOUniform = glGetUniformLocation(programID,"texAO"); // Get uniform ID
	GLuint texNormalUniform = glGetUniformLocation(programID,"texNormal"); // Get uniform ID
	GLuint texSpecUniform = glGetUniformLocation(programID,"texSpec"); // Get uniform ID

	GLuint matrixUniform = glGetUniformLocation(programID,"matrixMVP"); // Get uniform ID
	GLuint matrixModelUniform = glGetUniformLocation(programID,"matrixModel"); // Get uniform ID
	GLuint matrixViewUniform = glGetUniformLocation(programID,"matrixView"); // Get uniform ID
	GLuint matrixProjectUniform = glGetUniformLocation(programID,"matrixProject"); // Get uniform ID
	GLuint matrixModelViewUniform = glGetUniformLocation(programID,"matrixModelView3x3"); // Get uniform ID

	// Get a handle for our "LightPosition" uniform
	glUseProgram(programID);
	GLuint lightID = glGetUniformLocation(programID,"lightPositionWorldspace");


	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use shaders
		glUseProgram(programID);


		// Projection matrix: 45° Field of View. 4:3 ratio. display range : 0.1 unit <-> 100 units.
		glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(45.0f),(float)SCREEN_WIDTH/(float)SCREEN_HEIGHT,0.1f,100.0f);
		// Camera matrix: Camera is at (4,3,3), in World Space. looks at the origin. Head is up (set to 0,-1,0 to look upside-down).
		glm::mat4 ViewMatrix = glm::lookAt(glm::vec3(0,16,20),glm::vec3(0,6,0),glm::vec3(0,1,0));
		// Model matrix : glm::rotate( angle_in_degrees, myRotationAxis )
		glm::mat4 ModelMatrix = glm::rotate(0.5f*CurrentTime(),glm::vec3(0,1,0)) * glm::scale(glm::vec3(1.0f)) * glm::mat4(1.0f);
		// glm::mat4 lightModelMatrix = glm::rotate(-0.5f*CurrentTime(),glm::vec3(0,1,0)) * glm::scale(glm::vec3(0.25)) * glm::mat4(1.0f);
		glm::mat4 ModelViewMatrix = ViewMatrix * ModelMatrix;
		glm::mat3 ModelView3x3Matrix = glm::mat3(ModelViewMatrix);
		// Our ModelViewProjection : multiplication of our 3 matrices
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix; // Remember, matrix multiplication is the other way around
		// Send transformation to the currently bound shader,
		// in the "MVP" uniform
		glUniformMatrix4fv(matrixUniform,1,GL_FALSE,&MVP[0][0]);
		glUniformMatrix4fv(matrixModelUniform,1,GL_FALSE,&ModelMatrix[0][0]);
		glUniformMatrix4fv(matrixViewUniform,1,GL_FALSE,&ViewMatrix[0][0]);
		glUniformMatrix4fv(matrixProjectUniform,1,GL_FALSE,&ProjectionMatrix[0][0]);
		glUniformMatrix3fv(matrixModelViewUniform, 1, GL_FALSE, &ModelView3x3Matrix[0][0]);

		glm::vec3 lightPos = glm::vec3(0,4,5);
		glUniform3f(lightID, lightPos.x, lightPos.y, lightPos.z);


		// Bind our diffuse texture in Texture Unit 0
		updateTexture(texDiffuseID,texDiffuseUniform,0);
		updateTexture(texAOID,texAOUniform,1);
		updateTexture(texNormalID,texNormalUniform,2);
		updateTexture(texSpecID,texSpecUniform,3);

		// Draw model
		updateModel(vertexBuffer,uvBuffer,normalBuffer,tangentBuffer,bitangentBuffer,count);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window,GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	// Cleanup VBO
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &uvBuffer);
	glDeleteBuffers(1, &normalBuffer);
	glDeleteBuffers(1, &tangentBuffer);
	glDeleteBuffers(1, &bitangentBuffer);
	glDeleteTextures(1, &texDiffuseID);
	glDeleteTextures(1, &texAOID);
	glDeleteTextures(1, &texNormalID);
	glDeleteTextures(1, &texSpecID);
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
