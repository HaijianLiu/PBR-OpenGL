// Include standard libraries
#include <iostream>

// Include GLEW (include before gl.h and glfw.h)
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
// #include <glm/glm.hpp>
// #include <glm/gtx/transform.hpp>
// #include <glm/gtc/matrix_transform.hpp>

// Include header files
#include "input.hpp"

int getKeyboardPress(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		return KEY_UP;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		return KEY_DOWN;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		return KEY_LEFT;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		return KEY_RIGHT;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		return KEY_RIGHT;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		return KEY_RIGHT;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		return KEY_RIGHT;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		return KEY_RIGHT;

	return KEY_NONE;
}
