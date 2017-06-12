#ifndef shaderloader_hpp
#define shaderloader_hpp

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
GLuint loadShader(std::string vertexPath, std::string fragmentPath);


#endif /* shaderloader_hpp */
