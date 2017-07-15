#ifndef Mesh_hpp
#define Mesh_hpp

#include <vector>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

struct Texture {
	unsigned int id;
	std::string type;
	aiString path;
};

class Mesh {
public:
	/*  Mesh Data  */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	unsigned int vertexArrayID;

	/*  Functions  */
	// constructor
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	// render the mesh
	void Draw(Shader* shader);

private:
	/*  Render data  */
	unsigned int vertexBuffer, elementBuffer;

	/*  Functions    */
	// initializes all the buffer objects/arrays
	void setupMesh();
};

#endif /* Mesh_hpp */
