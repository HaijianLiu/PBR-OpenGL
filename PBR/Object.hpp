#ifndef Object_hpp
#define Object_hpp

#include "Model.hpp"

enum ObjectMovement {
	moveForward, moveBackward, moveLeft, moveRight,
	turnUp, turnDown, turnLeft, turnRight,
	scaleUp, scaleDown
};

class Object {
public:
	Model* model;
	Object(Model* model);
	void translate(float x, float y, float z);
	void scale(float x, float y, float z);
	void scale(float x);
	void rotate(glm::vec3 rotation);
	void rotate(float x, float y, float z);
	glm::vec3 getPosition();
	glm::mat4 getMatrixModel();
	void draw(Camera camera);

	// void updateInput(ObjectMovement move, float speed);

private:
	glm::vec3 position;
	glm::vec3 scales;
	glm::vec3 rotation;
};

#endif /* Object_hpp */
