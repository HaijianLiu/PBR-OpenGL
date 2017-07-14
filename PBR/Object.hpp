#ifndef Object_hpp
#define Object_hpp

class Object {
public:
	Object();
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
