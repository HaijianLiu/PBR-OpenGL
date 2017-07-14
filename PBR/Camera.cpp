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
	position   = glm::vec3(0.0,5.0,20.0);
	target     = glm::vec3(0.0,0.0,0.0);
	upside     = glm::vec3(0.0,1.0,0.0);
	field      = 45.0;
	ratio      = 4.0/3.0;
	rangeStart = 0.1;
	rangeEnd   = 100.0;
}

void Camera::translate(float x, float y, float z) {
	position = glm::vec3(x,y,z);
}
void Camera::setTarget(float x, float y, float z) {
	target = glm::vec3(x,y,z);
}
void Camera::setTarget(glm::vec3 tar) {
	target = tar;
}
void Camera::setUpside(glm::vec3 up) {
	upside = up;
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
	return glm::lookAt(position,target,upside);
}
