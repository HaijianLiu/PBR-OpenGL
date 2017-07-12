#ifndef Mesh_hpp
#define Mesh_hpp

struct Vertex {
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};


class Mesh {
public:
	/* Mesh Data */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int vertexArrayID;

	/* Functions */
	// constructor
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

private:
	/* Render data */
	unsigned int vertexBuffer, elementBuffer;

	/* Functions */
	// initializes all the buffer objects/arrays
	void setupMesh();
};


#endif /* Mesh_hpp */
