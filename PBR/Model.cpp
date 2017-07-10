// Include GLEW (include before gl.h and glfw.h)
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
// #include <glm/glm.hpp>
// #include <glm/gtx/transform.hpp>
// #include <glm/gtc/matrix_transform.hpp>

// Include header file
#include "objloader.hpp"
// Include Class
#include "Model.hpp"

// Class Model
Model::Model(const char* path) {
	// Create Vertex Array Object
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);
	// Load Model
	loadObj(path,vertexBuffer,uvBuffer,normalBuffer,tangentBuffer,bitangentBuffer,count);
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER,uvBuffer);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,(void*)0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER,normalBuffer);
	glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,(void*)0);
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER,tangentBuffer);
	glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,0,(void*)0);
	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER,bitangentBuffer);
	glVertexAttribPointer(4,3,GL_FLOAT,GL_FALSE,0,(void*)0);

	// Disable Bind
	// glDisableVertexAttribArray(0);
	// glDisableVertexAttribArray(1);
	// glDisableVertexAttribArray(2);
	// glDisableVertexAttribArray(3);
	// glDisableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);
}

Model::~Model() {
	glDeleteVertexArrays(1, &vertexArrayID);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &uvBuffer);
	glDeleteBuffers(1, &normalBuffer);
	glDeleteBuffers(1, &tangentBuffer);
	glDeleteBuffers(1, &bitangentBuffer);
}
