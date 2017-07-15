// Include standard headers
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

// Include GLEW (include before gl.h and glfw.h)
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
// #include <glm/gtx/transform.hpp>
// #include <glm/gtc/matrix_transform.hpp>

// Include assimp libaray
#include <assimp/Importer.hpp>

// Include class
#include "Shader.hpp"
#include "Mesh.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	// now that we have all the required data, set the vertex buffers and its attribute pointers.
	Mesh::setupMesh();
}

// render the mesh
void Mesh::draw(Shader* shader) {
	// bind appropriate textures
	// unsigned int diffuseNr  = 1;
	// unsigned int normalNr   = 1;
	// unsigned int metalNr    = 1;
	// unsigned int roughNr    = 1;
	// unsigned int ambientNr  = 1;
	for(unsigned int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		std::string name = textures[i].type;

		/* Muti same kind of textures */
		// // retrieve texture number (the N in diffuse_textureN)
		// std::stringstream ss;
		// std::string number;
		// if(name == UNIFORM_TEX_DIFFUSE)
		// 	ss << diffuseNr++; // transfer unsigned int to stream
		// else if(name == UNIFORM_TEX_NORMAL)
		// 	ss << normalNr++; // transfer unsigned int to stream
		// else if(name == UNIFORM_TEX_METAL)
		// 	ss << metalNr++; // transfer unsigned int to stream
		// else if(name == UNIFORM_TEX_ROUGH)
		// 	ss << roughNr++; // transfer unsigned int to stream
		// else if(name == UNIFORM_TEX_AO)
		// 	ss << ambientNr++; // transfer unsigned int to stream
		// number = ss.str();
		// // now set the sampler to the correct texture unit
		// glUniform1i(glGetUniformLocation(shader.programID, (name + number).c_str()), i);

		glUniform1i(glGetUniformLocation(shader->programID, name.c_str()), i);

		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	// draw mesh
	glBindVertexArray(vertexArrayID);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh() {
	// create buffers/arrays
	glGenVertexArrays(1, &vertexArrayID);
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &elementBuffer);

	glBindVertexArray(vertexArrayID);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	// vertex uv
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

	glBindVertexArray(0);
}
