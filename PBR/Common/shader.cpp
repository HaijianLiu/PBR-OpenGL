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
#include "shader.hpp"


GLuint loadShader(std::string VertexPath,std::string FragmentPath){

	// Set path
	std::string CurrentDir = __FILE__;
	std::string TargetDir = CurrentDir.substr(0,CurrentDir.rfind("/"));
	TargetDir = TargetDir.substr(0,TargetDir.rfind("/")) + "/Shader/";
	VertexPath = TargetDir + VertexPath;
	FragmentPath = TargetDir + FragmentPath;

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(VertexPath,std::ios::in);
	if(VertexShaderStream.is_open()){
		std::string Line = "";
		while(getline(VertexShaderStream,Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}else{
		printf("LoadShader: Impossible to open %s.\n",VertexPath.c_str());
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(FragmentPath,std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream,Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}else{
		printf("LoadShader: Impossible to open %s.\n",FragmentPath.c_str());
		getchar();
		return 0;
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("LoadShader: Compiling shader %s\n",VertexPath.c_str());
	const char* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID,1,&VertexSourcePointer,NULL);
	glCompileShader(VertexShaderID);
	// Check Vertex Shader
	glGetShaderiv(VertexShaderID,GL_COMPILE_STATUS,&Result);
	glGetShaderiv(VertexShaderID,GL_INFO_LOG_LENGTH,&InfoLogLength);
	if(InfoLogLength > 0){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID,InfoLogLength,NULL,&VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("LoadShader: Compiling shader %s\n",FragmentPath.c_str());
	const char* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID,1,&FragmentSourcePointer,NULL);
	glCompileShader(FragmentShaderID);
	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID,GL_COMPILE_STATUS,&Result);
	glGetShaderiv(FragmentShaderID,GL_INFO_LOG_LENGTH,&InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID,InfoLogLength,NULL,&FragmentShaderErrorMessage[0]);
		printf("%s\n",&FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("LoadShader: Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID,VertexShaderID);
	glAttachShader(ProgramID,FragmentShaderID);
	glLinkProgram(ProgramID);
	// Check the program
	glGetProgramiv(ProgramID,GL_LINK_STATUS,&Result);
	glGetProgramiv(ProgramID,GL_INFO_LOG_LENGTH,&InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID,InfoLogLength,NULL,&ProgramErrorMessage[0]);
		printf("%s\n",&ProgramErrorMessage[0]);
	}

	// Detach shader
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	// Delete shader
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}
