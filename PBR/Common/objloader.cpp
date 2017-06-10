// Include standard headers
#include <iostream>
#include <vector>

// Include GLM
#include <glm/glm.hpp>

// Include header file
#include "objloader.hpp"

bool loadObj(const char * path,std::vector<glm::vec3> & outVertices,std::vector<glm::vec2> & outUVs,std::vector<glm::vec3> & outNormals){
	// Set path
	std::string currentDir = __FILE__;
	std::string targetDir = currentDir.substr(0,currentDir.rfind("/"));
	targetDir = targetDir.substr(0,targetDir.rfind("/")) + "/Model/";
	path = targetDir.append(path).c_str();

	printf("LoadOBJ: Loading OBJ file %s...\n",path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempUVs;
	std::vector<glm::vec3> tempNormals;

	// Open the OBJ file
	FILE * file = fopen(path,"r");
	if(file == NULL){
		printf("LoadOBJ: Impossible to open %s.\n",path);
		getchar();
		return false;
	}

	// Read the OBJ file
	while (true){
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file,"%s",lineHeader);
		if (res == EOF){
			break; // EOF = End Of File. Quit the loop.
		}

		// parse lineHeader
		if (strcmp(lineHeader,"v") == 0) {
			glm::vec3 vertex;
			fscanf(file,"%f %f %f\n",&vertex.x,&vertex.y,&vertex.z);
			tempVertices.push_back(vertex);
		} else
		if (strcmp(lineHeader,"vn") == 0) {
			glm::vec3 normal;
			fscanf(file,"%f %f %f\n",&normal.x,&normal.y,&normal.z);
			tempNormals.push_back(normal);
		} else
		if (strcmp(lineHeader,"vt") == 0) {
			glm::vec2 uv;
			float uvZ;
			fscanf(file,"%f %f %f\n",&uv.x,&uv.y,&uvZ);
			tempUVs.push_back(uv);
		} else
		if (strcmp(lineHeader,"f") == 0) {

			unsigned int vertexIndex[4], normalIndex[4], uvIndex[4];

			int check34 = fscanf(file,"%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",&vertexIndex[0],&normalIndex[0],&uvIndex[0],&vertexIndex[1],&normalIndex[1],&uvIndex[1],&vertexIndex[2],&normalIndex[2],&uvIndex[2],&vertexIndex[3],&normalIndex[3],&uvIndex[3]);
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);

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
			fgets(stupidBuffer, 1000, file);
		}
	}

	printf("vertexIndices = %ld\n",vertexIndices.size());

	// Output OBJ Data for each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		// Get the attributes thanks to the index
		glm::vec3 vertex = tempVertices[vertexIndex-1];
		glm::vec2 uv = tempUVs[uvIndex-1];
		glm::vec3 normal = tempNormals[normalIndex-1];
		// Put the attributes in buffers
		outVertices.push_back(vertex);
		outUVs.push_back(uv);
		outNormals.push_back(normal);
	}


	fclose(file);
	return true;
}
