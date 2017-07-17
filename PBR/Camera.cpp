// Include standard headers
#include <iostream>
#include <vector>

// Include GLEW (include before gl.h and glfw.h)
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Include class
#include "Camera.hpp"


Camera::Camera() {
	// Camera Position
	position    = glm::vec3(0.0,10.0,20.0);
	cameraFront = glm::vec3(0.0,0.0,-1.0);
	cameraUp    = glm::vec3(0.0,1.0,0.0);
	cameraRight = glm::vec3(1.0,0.0,0.0);
	worldFront  = glm::vec3(0.0,0.0,-1.0);
	worldUp     = glm::vec3(0.0,1.0,0.0);
	worldRight  = glm::vec3(1.0,0.0,0.0);
	// Eular Angles
	yaw   = -90.0;
	pitch = 0.0;
	// Moment options
	moveSpeed = 8.0;
	rotateSensitivity = 40.0;
	// Camera Attributes
	field      = 45.0;
	ratio      = 800.0/600.0;
	rangeStart = 0.1;
	rangeEnd   = 100.0;
}

void Camera::translate(float x, float y, float z) {
	position = glm::vec3(x,y,z);
}
void Camera::setUpside(glm::vec3 up) {
	// upside = up;
}
void Camera::setField(float degree) {
	field = degree;
}
void Camera::setRatio(float rate) {
	ratio = rate;
}
void Camera::setRange(float start, float end) {
	rangeStart = start;
	rangeEnd = end;
}

glm::mat4 Camera::getMatrixProjection() {
	// Projection matrix (Field of View, ratio, display range : start unit <-> end units)
	return glm::perspective(glm::radians(field),ratio,rangeStart,rangeEnd);
}
glm::mat4 Camera::getMatrixView() {
	return glm::lookAt(position, position + cameraFront, cameraUp);
}
glm::vec3 Camera::getPosition() {
	return position;
}

void Camera::updateInput(GLFWwindow* window, float deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		position += worldFront * moveSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		position -= worldFront * moveSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		position -= worldRight * moveSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		position += worldRight * moveSpeed * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		pitch += rotateSensitivity * deltaTime;
		if (pitch > 40.0) {
			pitch = 40.0;
		}
		Camera::updateCameraVectors();
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		pitch -= rotateSensitivity * deltaTime;
		if (pitch < -40.0) {
			pitch = -40.0;
		}
		Camera::updateCameraVectors();
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		yaw -= rotateSensitivity * deltaTime;
		Camera::updateCameraVectors();
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		yaw += rotateSensitivity * deltaTime;
		Camera::updateCameraVectors();
	}
}

void Camera::updateCameraVectors() {
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
	cameraUp    = glm::normalize(glm::cross(cameraRight, cameraFront));
	worldRight  = cameraRight;
	worldFront  = glm::normalize(glm::cross(worldUp, worldRight));
}
