#ifndef Camera_hpp
#define Camera_hpp

class Camera {
public:
	Camera();
	void translate(float x, float y, float z);
	void setTarget(float x, float y, float z);
	void setTarget(glm::vec3 tar);
	void setUpside(glm::vec3 up);
	void setField(float degree);
	void setRatio(float ratio);
	void setRange(float start, float end);
	glm::mat4 getMatrixProjection();
	glm::mat4 getMatrixView();
	glm::vec3 getPosition();

private:
	glm::vec3 position; // in World Space
	glm::vec3 target; // in World Space
	glm::vec3 upside; // Head is up (0,1,0)
	float field; // Degree° Field of View
	float ratio;
	float rangeStart;
	float rangeEnd;
};

#endif /* Camera_hpp */
