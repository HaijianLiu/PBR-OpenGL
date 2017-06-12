#ifndef objloader_hpp
#define objloader_hpp

/*------------------------------------------------------------------------------
< Load OBJ file >

1. load data from the obj file
2. Give the data to OpenGL for generating buffers
................................................................................
Return:

3 buffers: vertexBuffer, uvBuffer, normalBuffer (for attributes)
1 count (for draw triangles)
................................................................................
Example:

GLuint vertexBuffer, uvBuffer, normalBuffer;
unsigned long count;
loadObj(OBJ_FILE,vertexBuffer,uvBuffer,normalBuffer,count);
------------------------------------------------------------------------------*/
bool loadObj(const char* path, GLuint& vertexBuffer, GLuint& uvBuffer, GLuint& normalBuffer, unsigned long& count);


#endif /* objloader_hpp */
