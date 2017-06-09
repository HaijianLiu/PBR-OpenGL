// Include standard headers
#include <iostream>
#include <vector>

// Include GLEW (include before gl.h and glfw.h)
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <sys/time.h>
#include <math.h>

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
  window = glfwCreateWindow(800,600,"PBR",NULL,NULL);
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

  // Set input mode GLFW_STICKY_KEYS
  glfwSetInputMode(window,GLFW_STICKY_KEYS,GL_TRUE);

  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  do {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

  } while(glfwGetKey(window,GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

  // Close OpenGL window and terminate GLFW
  glfwTerminate();

  return 0;
}
