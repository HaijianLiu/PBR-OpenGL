#ifndef Object_hpp
#define Object_hpp

#include "Model.hpp"

class Object {
public:
	Model* model;
	Object(Model* model);
	void translate(float x, float y, float z);
	void scale(float x, float y, float z);
	void scale(float x);
	void rotate(float angle, glm::vec3 axis);
	glm::vec3 getPosition();
	glm::mat4 getMatrixModel();

private:
	glm::vec3 position;
	glm::vec3 scales;
	glm::vec3 rotationAxis;
	float rotationAngle;
};

#endif /* Object_hpp */
