// Include standard headers
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

// Include GLEW (include before gl.h and glfw.h)
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
// #include <glm/gtx/transform.hpp>
// #include <glm/gtc/matrix_transform.hpp>

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


/*------------------------------------------------------------------------------
< Shader Class >
------------------------------------------------------------------------------*/
Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	programID = loadShader(vertexPath,fragmentPath);
}
Shader::~Shader() {
	glDeleteProgram(programID);
}
// activate the shader
void Shader::use() {
	glUseProgram(programID);
}
// utility uniform functions
void Shader::setBool(const char* name, bool value) {
	glUniform1i(glGetUniformLocation(programID,name), (int)value );
}
void Shader::setInt(const char* name, int value) {
	glUniform1i(glGetUniformLocation(programID, name), value);
}
void Shader::setFloat(const char* name, float value) {
	glUniform1f(glGetUniformLocation(programID, name), value);
}
void Shader::setVec2(const char* name, glm::vec2 value) {
	glUniform2fv(glGetUniformLocation(programID, name), 1, &value[0]);
}
void Shader::setVec2(const char* name, float x, float y) {
	glUniform2f(glGetUniformLocation(programID, name), x, y);
}
void Shader::setVec3(const char* name, glm::vec3 value) {
	glUniform3fv(glGetUniformLocation(programID, name), 1, &value[0]);
}
void Shader::setVec3(const char* name, float x, float y, float z) {
	glUniform3f(glGetUniformLocation(programID, name), x, y, z);
}
void Shader::setVec4(const char* name, glm::vec4 value) {
	glUniform4fv(glGetUniformLocation(programID, name), 1, &value[0]);
}
void Shader::setVec4(const char* name, float x, float y, float z, float w) {
	glUniform4f(glGetUniformLocation(programID, name), x, y, z, w);
}
void Shader::setMat2(const char* name, glm::mat2 mat) {
	glUniformMatrix2fv(glGetUniformLocation(programID, name), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat3(const char* name, glm::mat3 mat) {
	glUniformMatrix3fv(glGetUniformLocation(programID, name), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat4(const char* name, glm::mat4 mat) {
	glUniformMatrix4fv(glGetUniformLocation(programID, name), 1, GL_FALSE, &mat[0][0]);
}
