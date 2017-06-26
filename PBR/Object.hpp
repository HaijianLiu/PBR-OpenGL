#ifndef Object_hpp
#define Object_hpp

class Object {
private:
	glm::vec3 position;
	glm::vec3 scales;
	glm::vec3 rotationAxis;
	float rotationAngle;

public:
	Object ();
	void translate(float x, float y, float z);
	void scale(float x, float y, float z);
	void rotate(float angle, glm::vec3 axis);
	glm::mat4 getMatrixModel();
};

#endif /* Object_hpp */
