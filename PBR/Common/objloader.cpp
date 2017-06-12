// Include standard headers
#include <iostream>
#include <vector>

// Include GLEW (include before gl.h and glfw.h)
#include <GL/glew.h>
// Include GLM
#include <glm/glm.hpp>

// Include header file
#include "objloader.hpp"


bool loadObj(const char* path, GLuint& vertexBuffer, GLuint& uvBuffer, GLuint& normalBuffer, unsigned long& count) {
	// Set path
	std::string currentDir = __FILE__;
	std::string targetDir = currentDir.substr(0,currentDir.rfind("/"));
	targetDir = targetDir.substr(0,targetDir.rfind("/")) + "/Model/";
	path = targetDir.append(path).c_str();

	// Prepare to open the file
	printf("[loadObj] Loading OBJ file...<%s>\n",path); // Debug information

	// Open the OBJ file
	FILE* file = fopen(path,"rb");
	if (file == NULL) {
		printf("[loadObj] Impossible to open: <%s>\n",path); // Debug information
		getchar();
		return false;
	}


	// Read the OBJ file
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempUVs;
	std::vector<glm::vec3> tempNormals;

	while (true) {
		// Check header
		char lineHeader[128];

		// read the first word of the line
		int res = fscanf(file,"%s",lineHeader);

		// EOF = End Of File. Quit the loop.
		if (res == EOF) {
			break;
		}

		// Parse lineHeader
		// Load vertex data to vector<glm::vec3> tempVertices
		if (strcmp(lineHeader,"v") == 0) {
			glm::vec3 vertex;
			fscanf(file,"%f %f %f\n",&vertex.x,&vertex.y,&vertex.z);
			tempVertices.push_back(vertex);
		} else
		// Load UV data to vector<glm::vec2> tempUVs
		if (strcmp(lineHeader,"vt") == 0) {
			glm::vec2 uv;
			float uvZ;
			fscanf(file,"%f %f %f\n",&uv.x,&uv.y,&uvZ);
			// uv.y = -uv.y;  // Invert V coordinate when use DDS texture.
			tempUVs.push_back(uv);
		} else
		// Load normal data to vector<glm::vec2> tempNormals
		if (strcmp(lineHeader,"vn") == 0) {
			glm::vec3 normal;
			fscanf(file,"%f %f %f\n",&normal.x,&normal.y,&normal.z);
			tempNormals.push_back(normal);
		} else
		// Load index data to vector<unsigned int> vertexIndices, uvIndices, normalIndices
		if (strcmp(lineHeader,"f") == 0) {
			unsigned int vertexIndex[4], normalIndex[4], uvIndex[4];
			// Check if here is a triangle or square data index. Divide the index into [0,1,2] and [0,2,3]
			int check34 = fscanf(file,"%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",&vertexIndex[0],&uvIndex[0],&normalIndex[0],&vertexIndex[1],&uvIndex[1],&normalIndex[1],&vertexIndex[2],&uvIndex[2],&normalIndex[2],&vertexIndex[3],&uvIndex[3],&normalIndex[3]);
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			// if here is a square data index
			if (check34 == 12) {
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[2]);
				vertexIndices.push_back(vertexIndex[3]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[2]);
				normalIndices.push_back(normalIndex[3]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[2]);
				uvIndices.push_back(uvIndex[3]);
			}
		} else {
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer,1000,file);
		}
	}

	// close OBJ file
	fclose(file);


	// The number of triangles
	count = vertexIndices.size();
	printf("[loadObj] The number of triangles = %ld\n",vertexIndices.size());  // Debug information


	// Output OBJ Data for each vertex of each triangle
	std::vector<glm::vec3> vertexData;
	std::vector<glm::vec2> uvData;
	std::vector<glm::vec3> normalData;

	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		// Get the attributes thanks to the index and Put the attributes in buffers
		vertexData.push_back(tempVertices[vertexIndex-1]);
		uvData.push_back(tempUVs[uvIndex-1]);
		normalData.push_back(tempNormals[normalIndex-1]);
	}

	// Load into VBO
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,vertexData.size()*sizeof(glm::vec3),&vertexData[0],GL_STATIC_DRAW);
	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,uvBuffer);
	glBufferData(GL_ARRAY_BUFFER,uvData.size()*sizeof(glm::vec2),&uvData[0],GL_STATIC_DRAW);
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,normalBuffer);
	glBufferData(GL_ARRAY_BUFFER,normalData.size()*sizeof(glm::vec3),&normalData[0],GL_STATIC_DRAW);

	return true;
}
