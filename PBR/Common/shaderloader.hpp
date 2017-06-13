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


#endif /* shaderloader_hpp */
