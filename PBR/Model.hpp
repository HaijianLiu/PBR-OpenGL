#ifndef Model_hpp
#define Model_hpp

/*----------------------------------------------------------------------------*/
class Model {
public:
	Model (const char* path);
	virtual ~Model ();
	unsigned int vertexArrayID, vertexBuffer, uvBuffer, normalBuffer, tangentBuffer, bitangentBuffer;
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
vertexArrayID, vertexBuffer, uvBuffer, normalBuffer, tangentBuffer, bitangentBuffer : GLint buffers for attributes
count: number of triangles
................................................................................
Example:

Model* model = new Model(path);
...
delete model;
------------------------------------------------------------------------------*/

#endif /* Model_hpp */
