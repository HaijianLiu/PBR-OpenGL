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
#define FILE_DIFFUSE_TGA    "WPNT_MK2Grenade_Base_Color.tga"
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


	// Load Model
	GLuint vertexBuffer, uvBuffer, normalBuffer, tangentBuffer, bitangentBuffer;
	unsigned long count;
	loadObj(FILE_OBJ,vertexBuffer,uvBuffer,normalBuffer,tangentBuffer,bitangentBuffer,count);


	// Create and compile GLSL program from the shaders
	GLuint programID = loadShader(VERTEXSHADER_GLSL,FRAGMENTSHADER_GLSL);


	// Load Texture
	GLuint texDiffuseID = loadTGA(FILE_DIFFUSE_TGA);
	GLuint texAOID      = loadTGA(FILE_AO_TGA);
	GLuint texNormalID  = loadTGA(FILE_NORMAL_TGA);
	GLuint texSpecID    = loadTGA(FILE_SPEC_TGA);


	// Create Camera
	Camera* camera = new Camera;
	// Create Object
	Object* grenadeMK2 = new Object;


	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		grenadeMK2->rotate(0.5f*CurrentTime(),glm::vec3(0,1,0));
		camera->setTarget(grenadeMK2->getPosition() + glm::vec3(0,6,0));

		// renderScreen(Camera* camera);

		// Use shaders
		glUseProgram(programID);

		glm::mat4 matrixProjection = camera->getMatrixProjection();
		glm::mat4 matrixView       = camera->getMatrixView();
		glm::mat4 matrixModel      = grenadeMK2->getMatrixModel();
		glm::mat4 matrixMVP        = matrixProjection * matrixView * matrixModel;
		// Get uniform
		GLuint matrixMVPUniform   = glGetUniformLocation(programID,"matrixMVP"); // Get uniform ID
		GLuint matrixModelUniform = glGetUniformLocation(programID,"matrixModel"); // Get uniform ID
		// Send transformation to the currently bound shader,
		glUniformMatrix4fv(matrixMVPUniform,1,GL_FALSE,&matrixMVP[0][0]);
		glUniformMatrix4fv(matrixModelUniform,1,GL_FALSE,&matrixModel[0][0]);

		// Get uniform
		GLuint texDiffuseUniform  = glGetUniformLocation(programID,"texDiffuse"); // Get uniform ID
		GLuint texAOUniform       = glGetUniformLocation(programID,"texAO"); // Get uniform ID
		GLuint texNormalUniform   = glGetUniformLocation(programID,"texNormal"); // Get uniform ID
		GLuint texSpecUniform     = glGetUniformLocation(programID,"texSpec"); // Get uniform ID
		// Bind texture in Texture Unit 0 ~
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
	// Delete Object
	delete camera;
	delete grenadeMK2;
	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

float CurrentTime() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (float)(tv.tv_sec-startTime.tv_sec)+(tv.tv_usec-startTime.tv_usec)/1000000.0f;
}
