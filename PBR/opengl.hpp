#ifndef opengl_hpp
#define opengl_hpp

// Include standard headers
#include <iostream>

// Include GLEW (include before gl.h and glfw.h)
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>


// Created a OpenGL Window: (Default settings)
// GLFWwindow* window = createWindow("window name",screenWidth,screenHeight);
GLFWwindow* createWindow(const char* name, int screenWidth, int screenHeight);


#endif /* opengl_hpp */
