// Include standard headers
#include <iostream>

// Include header file
#include "opengl.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


// Define settings
#define WINDOW_NAME "PBR"
#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (600)
#define VERTEXSHADER_GLSL   "vertexshader.glsl"
#define FRAGMENTSHADER_GLSL "fragmentshader.glsl"
// #define FILE_OBJ            "WPN_MK2Grenade.obj"
// #define FILE_DIFFUSE_TGA    "WPNT_MK2Grenade_Base_Color.tga"
// #define FILE_NORMAL_TGA     "WPNT_MK2Grenade_Normal_DirectX.tga"
// #define FILE_METAL_TGA      "WPNT_MK2Grenade_Metallic.tga"
// #define FILE_ROUGH_TGA      "WPNT_MK2Grenade_Roughness.tga"
// #define FILE_AO_TGA         "WPNT_MK2Grenade_Ambient_occlusion.tga"
#define FILE_OBJ            "WPN_AKM.obj"
#define FILE_DIFFUSE_TGA    "WPNT_AKM_Base_Color.tga"
#define FILE_NORMAL_TGA     "WPNT_AKM_DirectX.tga"
#define FILE_METAL_TGA      "WPNT_AKM_Metallic.tga"
#define FILE_ROUGH_TGA      "WPNT_AKM_Roughness.tga"
#define FILE_AO_TGA         "WPNT_AKM_Ambient_occlusion.tga"



int main(void) {

	// initialize start time
	initTime();
	// Created a OpenGL Window: (Default settings)
	GLFWwindow* window = createWindow(WINDOW_NAME,SCREEN_WIDTH,SCREEN_HEIGHT);
	// Create Vertex Array Object
	GLuint vertexArrayID = getVertexArray();

	// Create and compile GLSL program from the shaders
	GLuint shaderPBR = loadShader(VERTEXSHADER_GLSL,FRAGMENTSHADER_GLSL);

	GLuint shaderHDR  = loadShader("hdrVertex.glsl","hdrFragment.glsl");

	// Load Model & Texture
	Model* grenadeMK2Model        = new Model(FILE_OBJ);
	TexturePBR* grenadeMK2Texture = new TexturePBR(FILE_DIFFUSE_TGA,FILE_NORMAL_TGA,FILE_METAL_TGA,FILE_ROUGH_TGA,FILE_AO_TGA);

	// Model* cubeModel              = new Model("cube.obj");


	// Create Camera & Object
	Camera* camera     = new Camera;
	Object* grenadeMK2 = new Object;

	// Object* cube       = new Object;


// // =============================================================================
// 	stbi_set_flip_vertically_on_load(true);
// 	int width, height, nrComponents;
// 	float *data = stbi_loadf("/Users/haijian/Documents/OpenGL/PBR/PBR/Texture/hdrvfx_0012_sand_v11_Ref.hdr", &width, &height, &nrComponents, 0);
//
// 	unsigned int hdrTexture;
// 	if (data)
// 	{
// 		glGenTextures(1, &hdrTexture);
// 		glBindTexture(GL_TEXTURE_2D, hdrTexture);
// 		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);
//
// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
// 		stbi_image_free(data);
// 	}
// 	else
// 	{
// 		std::cout << "Failed to load HDR image." << std::endl;
// 	}
// // =============================================================================


	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		grenadeMK2->scale(0.3);
		// Update object and camera position
		grenadeMK2->rotate(0.3 * currentTime(),glm::vec3(0,1,0));


		camera->setTarget(grenadeMK2->getPosition() + glm::vec3(0,0,0));
		// rendering object using (model,texture,shader) in the view of camera
		rendering(grenadeMK2,grenadeMK2Model,grenadeMK2Texture,shaderPBR,camera);


		// cube->scale(5.0,5.0,5.0);
		// cube->translate(0,6,0);
		// rendering(cube,cubeModel,hdrTexture,shaderHDR,camera);


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
