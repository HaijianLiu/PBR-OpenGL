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
#include "timecontrol.hpp"
// Include Class
#include "Object.hpp"
#include "Camera.hpp"

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

vertexBuffer, uvBuffer, normalBuffer, tangentBuffer, bitangentBuffer : GLint buffers for attributes
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


/*----------------------------------------------------------------------------*/
class Model {
public:
	Model (const char* path);
	virtual ~Model ();
	GLuint vertexBuffer, uvBuffer, normalBuffer, tangentBuffer, bitangentBuffer;
	unsigned long count;
};
/*------------------------------------------------------------------------------
< Model Class >

Load model file
Store buffers
~Delete buffers
................................................................................
Members:

const char* path : model file path
vertexBuffer, uvBuffer, normalBuffer, tangentBuffer, bitangentBuffer : GLint buffers for attributes
count: number of triangles
................................................................................
Example:

Model* model = new Model(path);
...
delete model;
------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
class Texture {
public:
	Texture (const char* diffusePath, const char* normalPath, const char* specPath, const char* aoPath);
	virtual ~Texture ();
	GLuint texDiffuseID, texNormalID, texSpecID, texAOID;
};
/*------------------------------------------------------------------------------
< Texture Class >

Load Textures
Store textureIDs
~Delete textureIDs
................................................................................
Members:

const char* diffusePath, normalPath, specPath, aoPath : texture file path
GLuint texDiffuseID, texNormalID, texSpecID, texAOID : textureIDs
................................................................................
Example:

Texture* texture = new Texture(diffusePath, normalPath, specPath, aoPath);
...
delete texture;
------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
void rendering(Object* object, Model* model, Texture* texture, GLuint shader, Camera* camera);
/*------------------------------------------------------------------------------
< Rendering Object >

Use shader
Calculate matrixs and send to shader uniform
Send Textures to shader uniform
updateTexture(...)
updateModel(...)
................................................................................
Parameters:

Object* object, Model* model, Texture* texture, GLuint shader
Camera* camera : from the render view
................................................................................
Example:

rendering(object,model,texture,shader,camera);
------------------------------------------------------------------------------*/

#endif /* opengl_hpp */
