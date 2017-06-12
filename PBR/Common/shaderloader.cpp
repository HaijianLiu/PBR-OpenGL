// Include standard headers
#include <iostream>
#include <fstream>
#include <vector>

// Include GLEW (include before gl.h and glfw.h)
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>

// Include header file
#include "shaderloader.hpp"


GLuint loadShader(std::string vertexPath, std::string fragmentPath) {

	// Set path
	std::string currentDir = __FILE__;
	std::string targetDir = currentDir.substr(0,currentDir.rfind("/"));
	targetDir = targetDir.substr(0,targetDir.rfind("/")) + "/Shader/";
	vertexPath = targetDir + vertexPath;
	fragmentPath = targetDir + fragmentPath;

	// Create the shaders
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string vertexShaderCode;
	std::ifstream vertexShaderStream(vertexPath,std::ios::in);
	if (vertexShaderStream.is_open()) {
		std::string line = "";
		while (getline(vertexShaderStream,line)) {
			vertexShaderCode += "\n" + line;
		}
		vertexShaderStream.close();
	} else {
		printf("[loadShader] Impossible to open: <%s>\n",vertexPath.c_str());
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string fragmentShaderCode;
	std::ifstream fragmentShaderStream(fragmentPath,std::ios::in);
	if (fragmentShaderStream.is_open()) {
		std::string line = "";
		while (getline(fragmentShaderStream,line)) {
			fragmentShaderCode += "\n" + line;
		}
		fragmentShaderStream.close();
	} else {
		printf("[loadShader] Impossible to open: <%s>\n",fragmentPath.c_str());
		return 0;
	}

	GLint result = GL_FALSE;
	int infoLogLength;

	// Compile Vertex Shader
	printf("[loadShader] Compiling shader: <%s>\n",vertexPath.c_str());
	const char* vertexSourcePointer = vertexShaderCode.c_str();
	glShaderSource(vertexShaderID,1,&vertexSourcePointer,NULL);
	glCompileShader(vertexShaderID);
	// Check Vertex Shader
	glGetShaderiv(vertexShaderID,GL_COMPILE_STATUS,&result);
	glGetShaderiv(vertexShaderID,GL_INFO_LOG_LENGTH,&infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> vertexShaderErrorMessage(infoLogLength+1);
		glGetShaderInfoLog(vertexShaderID,infoLogLength,NULL,&vertexShaderErrorMessage[0]);
		printf("%s\n", &vertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("[loadShader] Compiling shader: <%s>\n",fragmentPath.c_str());
	const char* fragmentSourcePointer = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderID,1,&fragmentSourcePointer,NULL);
	glCompileShader(fragmentShaderID);
	// Check Fragment Shader
	glGetShaderiv(fragmentShaderID,GL_COMPILE_STATUS,&result);
	glGetShaderiv(fragmentShaderID,GL_INFO_LOG_LENGTH,&infoLogLength);
	if (infoLogLength > 0){
		std::vector<char> fragmentShaderErrorMessage(infoLogLength+1);
		glGetShaderInfoLog(fragmentShaderID,infoLogLength,NULL,&fragmentShaderErrorMessage[0]);
		printf("%s\n",&fragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("[loadShader] Linking program.\n");
	GLuint programID = glCreateProgram();
	glAttachShader(programID,vertexShaderID);
	glAttachShader(programID,fragmentShaderID);
	glLinkProgram(programID);
	// Check the program
	glGetProgramiv(programID,GL_LINK_STATUS,&result);
	glGetProgramiv(programID,GL_INFO_LOG_LENGTH,&infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> programErrorMessage(infoLogLength+1);
		glGetProgramInfoLog(programID,infoLogLength,NULL,&programErrorMessage[0]);
		printf("%s\n",&programErrorMessage[0]);
	}

	// Detach shader
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	// Delete shader
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return programID;
}
