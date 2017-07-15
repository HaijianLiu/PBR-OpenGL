#ifndef Shader_hpp
#define Shader_hpp

#define UNIFORM_TEX_DIFFUSE  "texDiffuse"
#define UNIFORM_TEX_NORMAL   "texNormal"
#define UNIFORM_TEX_METAL    "texMetal"
#define UNIFORM_TEX_ROUGH    "texRough"
#define UNIFORM_TEX_AO       "texAO"
#define UNIFORM_MATRIX_MVP   "matrixMVP"
#define UNIFORM_MATRIX_MODEL "matrixModel"

/*----------------------------------------------------------------------------*/
class Shader {
public:
	unsigned int programID;
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

private:
	unsigned int loadShader(std::string vertexPath, std::string fragmentPath);
};
/*------------------------------------------------------------------------------
< Shader Class >

Load vertexshader and fragmentshader
Delete vertexshader and fragmentshader
Create uniforms
................................................................................
Members:

unsigned int programID
................................................................................
Example:

Shader* shader = new Shader(vertexPath,fragmentPath);
...
delete shader;

Shader shader = Shader(vertexPath,fragmentPath);
------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
// unsigned int loadShader(std::string vertexPath, std::string fragmentPath);
/*------------------------------------------------------------------------------
< Load Shaders >

Load vertexshader and fragmentshader
Create and compile GLSL program from the shaders
................................................................................
Return:

unsigned int programID
................................................................................
Example:

unsigned int programID = loadShader("vertexshader.glsl","fragmentshader.glsl");
------------------------------------------------------------------------------*/

#endif /* shaderloader_hpp */
