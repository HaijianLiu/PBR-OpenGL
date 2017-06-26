// Include standard headers
#include <iostream>
#include <sys/time.h>

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


	// Create and compile GLSL program from the shaders
	GLuint shaderPBR = loadShader(VERTEXSHADER_GLSL,FRAGMENTSHADER_GLSL);

	// Load Model & Texture
	Model* grenadeMK2Model     = new Model(FILE_OBJ);
	Texture* grenadeMK2Texture = new Texture(FILE_DIFFUSE_TGA,FILE_NORMAL_TGA,FILE_SPEC_TGA,FILE_AO_TGA);

	// Create Camera & Object
	Camera* camera     = new Camera;
	Object* grenadeMK2 = new Object;


	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Update object and camera position
		grenadeMK2->rotate(0.5f*CurrentTime(),glm::vec3(0,1,0));
		camera->setTarget(grenadeMK2->getPosition() + glm::vec3(0,6,0));
		// rendering object using (model,texture,shader) in the view of camera
		rendering(grenadeMK2,grenadeMK2Model,grenadeMK2Texture,shaderPBR,camera);


		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window,GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	// Cleanup VBO
	delete grenadeMK2Model;
	delete grenadeMK2Texture;
	// Delete Object
	delete camera;
	delete grenadeMK2;
	// Delete shader
	glDeleteProgram(shaderPBR);
	// Close OpenGL window and terminate GLFW
	glDeleteVertexArrays(1, &vertexArrayID);
	glfwTerminate();

	return 0;
}

float CurrentTime() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (float)(tv.tv_sec-startTime.tv_sec)+(tv.tv_usec-startTime.tv_usec)/1000000.0f;
}
