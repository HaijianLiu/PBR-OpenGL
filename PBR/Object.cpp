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
#include "Camera.hpp"
#include "Object.hpp"

Object::Object(Model* model) {
	position      = glm::vec3(0.0,0.0,0.0);
	scales        = glm::vec3(1.0,1.0,1.0);
	rotation      = glm::vec3(0.0,0.0,0.0);
	this->model = model;
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
void Object::rotate(glm::vec3 rotation) {
	this->rotation = rotation;
}
void Object::rotate(float x, float y, float z) {
	rotation = glm::vec3(x,y,z);
}

glm::vec3 Object::getPosition() {
	return position;
}

glm::mat4 Object::getMatrixModel() {
	return glm::rotate(rotation.z,glm::vec3(0.0,0.0,1.0)) * glm::rotate(rotation.y,glm::vec3(0.0,1.0,0.0)) * glm::rotate(rotation.x,glm::vec3(1.0,0.0,0.0)) * glm::scale(scales) * glm::translate(position);
}

void Object::draw(Camera camera) {
	model->shader->use();
	model->shader->setMat4(UNIFORM_MATRIX_MODEL, Object::getMatrixModel());
	model->shader->setMat4(UNIFORM_MATRIX_MVP, camera.getMatrixProjection() * camera.getMatrixView() * Object::getMatrixModel());
	model->shader->setVec3(UNIFORM_EYE_WORLDSPACE, camera.getPosition());
	model->draw();
}

// void Object::updateInput(ObjectMovement move, float speed) {
// 	// Turn Movement
// 	if (move == turnUp)
// 		rotation.x -= speed;
// 	if (move == turnDown)
// 		rotation.x += speed;
// 	if (move == turnLeft)
// 		rotation.y -= speed;
// 	if (move == turnRight)
// 		rotation.y += speed;
// }
