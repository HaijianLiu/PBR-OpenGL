// Include standard headers
#include <iostream>
#include <vector>
#include <sys/time.h>
#include <math.h>

// Include GLEW (include before gl.h and glfw.h)
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Include header file
#include "shader.hpp"
#include "objloader.hpp"
#include "tgaloader.hpp"


// Define Macros
#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH (800)
#endif
#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT (600)
#endif
#ifndef OBJ_FILE
#define OBJ_FILE "WPN_MK2Grenade.obj"
#endif
#ifndef BASECOLOR_TGA_FILE
#define BASECOLOR_TGA_FILE "WPNT_MK2Grenade_Base_Color.tga"
#endif


// Time Function
struct timeval startTime;
float CurrentTime();

int main(void) {

	// Initialise GLFW
	if(!glfwInit())
	{
		fprintf(stderr,"Failed to initialize GLFW\n");
		getchar();
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // don't want the old OpenGL
	// Open a window and create its OpenGL context
	window = glfwCreateWindow(SCREEN_WIDTH,SCREEN_HEIGHT,"PBR",NULL,NULL);
	if(window == NULL){
		fprintf(stderr,"Failed to open GLFW window.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if(glewInit() != GLEW_OK) {
		fprintf(stderr,"Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	// initialize start time
	gettimeofday(&startTime, NULL);
	// Set input mode GLFW_STICKY_KEYS
	glfwSetInputMode(window,GLFW_STICKY_KEYS,GL_TRUE);
	// Dark blue background
	glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
	// Create Vertex Array Object
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint ProgramID = LoadShaders("vertexshader.glsl","fragmentshader.glsl");

	// Read OBJ file
	std::vector<glm::vec3> objvertices;
	std::vector<glm::vec2> objuvs;
	std::vector<glm::vec3> objnormals;
	loadObj(OBJ_FILE,objvertices,objuvs,objnormals);

	// GLubyte* data; // Hold All The Color Values For The Image.
	// GLuint width;  // The Width Of The Entire Image.
	// GLuint height; // The Height Of The Entire Image.
	// // Load A TGA File!
	// loadTGA(BASECOLOR_TGA_FILE,*data,width,height);

	// // Create one OpenGL texture
	// GLuint TextureID;
	// glGenTextures(1, &TextureID);
	// // "Bind" the newly created texture : all future texture functions will modify this texture
	// glBindTexture(GL_TEXTURE_2D, TextureID);
	// // Give the image to OpenGL
	// glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	// Load the texture
	GLuint TextureID = loadTGA(BASECOLOR_TGA_FILE);

	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureUniform  = glGetUniformLocation(ProgramID, "myTextureSampler");
	GLuint MatrixUniform  = glGetUniformLocation(ProgramID, "MVP");

	// Load into VBO
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER,vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER,objvertices.size()*sizeof(glm::vec3),&objvertices[0],GL_STATIC_DRAW);
	// Load into VBO
	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER,uvbuffer);
	glBufferData(GL_ARRAY_BUFFER,objuvs.size()*sizeof(glm::vec2),&objuvs[0],GL_STATIC_DRAW);

	// Load into VBO
	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER,normalbuffer);
	glBufferData(GL_ARRAY_BUFFER,objnormals.size()*sizeof(glm::vec3),&objnormals[0],GL_STATIC_DRAW);




	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use shaders
		glUseProgram(ProgramID);

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
		glUniformMatrix4fv(MatrixUniform,1,GL_FALSE,&MVP[0][0]);
		// Bind our diffuse texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureID);
		// Set our "DiffuseTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureUniform,0);

		// // Bind our diffuse texture in Texture Unit 0
		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, Texture);
		// // Set our "DiffuseTextureSampler" sampler to user Texture Unit 0
		// glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER,vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER,uvbuffer);
		glVertexAttribPointer(
			1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			2,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER,normalbuffer);
		glVertexAttribPointer(
			2,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES,0,objvertices.size()); // 3 indices starting at 0 -> 1 triangle
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);


		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} while(glfwGetKey(window,GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteTextures(1, &TextureID);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(ProgramID);
	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

float CurrentTime() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (float)(tv.tv_sec-startTime.tv_sec)+(tv.tv_usec-startTime.tv_usec)/1000000.0f;
}
