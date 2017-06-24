#ifndef objloader_hpp
#define objloader_hpp

/*----------------------------------------------------------------------------*/
bool loadObj(const char* path, GLuint& vertexBuffer, GLuint& uvBuffer, GLuint& normalBuffer, unsigned long& count);
/*------------------------------------------------------------------------------
< Load OBJ file >

Load data from the obj file
Give the data to OpenGL for generating buffers
................................................................................
Return:

3 buffers: vertexBuffer, uvBuffer, normalBuffer (for attributes)
1 count (for draw triangles)
................................................................................
Parameters:

vertexBuffer, uvBuffer, normalBuffer: GLint buffers for attributes
count: number of triangles
................................................................................
Example:

GLuint vertexBuffer, uvBuffer, normalBuffer;
unsigned long count;
loadObj("filename.obj",vertexBuffer,uvBuffer,normalBuffer,count);
------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
bool loadObj(const char* path, GLuint& vertexBuffer, GLuint& uvBuffer, GLuint& normalBuffer, GLuint& tangentBuffer, GLuint& bitangentBuffer, unsigned long& count);
/*------------------------------------------------------------------------------
< Load OBJ file >

Load data from the obj file
Give the data to OpenGL for generating buffers
................................................................................
Return:

3 buffers: vertexBuffer, uvBuffer, normalBuffer, tangentBuffer, bitangentBuffer (for attributes)
1 count (for draw triangles)
................................................................................
Parameters:

vertexBuffer, uvBuffer, normalBuffer, tangentBuffer, bitangentBuffer: GLint buffers for attributes
count: number of triangles
................................................................................
Example:

GLuint vertexBuffer, uvBuffer, normalBuffer, tangentBuffer, bitangentBuffer;
unsigned long count;
loadObj(FILE_OBJ,vertexBuffer,uvBuffer,normalBuffer,tangentBuffer,bitangentBuffer,count);
------------------------------------------------------------------------------*/


#endif /* objloader_hpp */
