#ifndef shaderloader_hpp
#define shaderloader_hpp

/*----------------------------------------------------------------------------*/
GLuint loadShader(std::string vertexPath, std::string fragmentPath);
/*------------------------------------------------------------------------------
< Load Shaders >

Load vertexshader and fragmentshader
Create and compile GLSL program from the shaders
................................................................................
Return:

GLuint programID
................................................................................
Example:

GLuint programID = loadShader("vertexshader.glsl","fragmentshader.glsl");
------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
class Shader {
public:
	GLuint programID;
	// constructor generates the shader on the fly
	Shader(const char* vertexPath, const char* fragmentPath);
	virtual ~Shader ();
	// activate the shader
	void use();
	// utility uniform functions
	void setBool(const char* name, bool value);
	void setInt(const char* name, int value);
	void setFloat(const char* name, float value);
	void setVec2(const char* name, glm::vec2 value);
	void setVec2(const char* name, float x, float y);
	void setVec3(const char* name, glm::vec3 value);
	void setVec3(const char* name, float x, float y, float z);
	void setVec4(const char* name, glm::vec4 value);
	void setVec4(const char* name, float x, float y, float z, float w);
	void setMat2(const char* name, glm::mat2 mat);
	void setMat3(const char* name, glm::mat3 mat);
	void setMat4(const char* name, glm::mat4 mat);
};
/*------------------------------------------------------------------------------
< Shader Class >

Load vertexshader and fragmentshader
Delete vertexshader and fragmentshader
Create uniforms
................................................................................
Members:

GLuint programID
................................................................................
Example:

Shader* shader = new Shader(vertexPath,fragmentPath);
...
delete shader;
------------------------------------------------------------------------------*/


#endif /* shaderloader_hpp */
