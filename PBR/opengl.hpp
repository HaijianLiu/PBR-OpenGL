#ifndef opengl_hpp
#define opengl_hpp

// Include GLEW (include before gl.h and glfw.h)
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Include header file
#include "shaderloader.hpp"
#include "objloader.hpp"
#include "tgaloader.hpp"

/*----------------------------------------------------------------------------*/
GLFWwindow* createWindow(const char* name, int screenWidth, int screenHeight);
/*------------------------------------------------------------------------------
< Created OpenGL Window >

Create a OpenGL window.
Default window settings : 4x antialiasing, OpenGL 3.3
Set Background color to (0.9,0.9,0.9,0.9)
Set input mode GLFW_STICKY_KEYS
Initialize GLEW.
Enable depth test.
Accept fragment if it closer to the camera than the former one.
Cull triangles which normal is not towards the camera.
................................................................................
Return:

GLFWwindow* window
................................................................................
Parameters:

const char* name : window name
int screenWidth : window width
int screenHeight : window height
................................................................................
Example:

GLFWwindow* window = createWindow("WindowName",screenWidth,screenHeight);
------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
GLuint getVertexArray();
/*------------------------------------------------------------------------------
< Create Vertex Array Object >
................................................................................
Return:

GLuint vertexArrayID
................................................................................
Example:

GLuint vertexArrayID = getVertexArray();
------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
void updateModel(GLuint vertexBuffer, GLuint uvBuffer, GLuint normalBuffer, unsigned long count);
/*------------------------------------------------------------------------------
< Draw Model >

Enable attributes buffer for vertex, uv, normal.
Draw the triangles by count.
Disable attributes
................................................................................
Parameters:

vertexBuffer, uvBuffer, normalBuffer: GLint buffers for attributes
count: number of triangles
................................................................................
Example:

updateModel(vertexBuffer,uvBuffer,normalBuffer,count);
------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
void updateModel(GLuint vertexBuffer, GLuint uvBuffer, GLuint normalBuffer, GLuint tangentBuffer, GLuint bitangentBuffer, unsigned long count);
/*------------------------------------------------------------------------------
< Draw Model >

Enable attributes buffer for vertex, uv, normal.
Draw the triangles by count.
Disable attributes
................................................................................
Parameters:

vertexBuffer, uvBuffer, normalBuffer: GLint buffers for attributes
count: number of triangles
................................................................................
Example:

updateModel(vertexBuffer,uvBuffer,normalBuffer,count);
------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
void updateTexture(GLuint textureID, GLint uniformID, int unit);
/*------------------------------------------------------------------------------
< Update Texture >

Active texture unit
Link textureID to GL_TEXTURE_2D
Link uniformID to unit
................................................................................
Parameters:

GLuint textureID, GLint uniformID
int unit : user texture unit number for uniform variables passed to vertex shader
................................................................................
Example:

updateTexture(textureID,uniformID,0);
------------------------------------------------------------------------------*/



#endif /* opengl_hpp */
