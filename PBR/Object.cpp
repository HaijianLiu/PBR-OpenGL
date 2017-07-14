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

// Include class
#include "Object.hpp"

Object::Object() {
	position      = glm::vec3(0.0,0.0,0.0);
	scales        = glm::vec3(1.0,1.0,1.0);
	rotationAxis  = glm::vec3(0.0,1.0,0.0);
	rotationAngle = 0.0;
}

void Object::translate(float x, float y, float z) {
	position = glm::vec3(x,y,z);
}
void Object::scale(float x, float y, float z) {
	scales = glm::vec3(x,y,z);
}
void Object::scale(float x) {
	scales = glm::vec3(x,x,x);
}
void Object::rotate(float angle, glm::vec3 axis) {
	rotationAxis  = axis;
	rotationAngle = angle;
}

glm::vec3 Object::getPosition() {
	return position;
}

glm::mat4 Object::getMatrixModel() {
	return glm::rotate(rotationAngle,rotationAxis) * glm::scale(scales) * glm::translate(position);
}
