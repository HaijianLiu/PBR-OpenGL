#ifndef tagloader_hpp
#define tagloader_hpp

/*----------------------------------------------------------------------------*/
GLuint loadTGA(const char* path);
/*------------------------------------------------------------------------------
< Load TGA file >

1. load data from the tga file
2. Give the data to OpenGL for generating texture
................................................................................
Return:

GLuint textureID
................................................................................
Example:

GLuint textureID = loadTGA("filename.tga");
................................................................................
Surpported:
2 uncompressed true-color (RGB or RGBA) image
3 uncompressed black-and-white (grayscale) image : (only return as red channel)
10 run-length encoded true-color (RGB or RGBA) image
11 run-length encoded black-and-white (grayscale) image : (only return as red channel)

Unsurpported:
1 uncompressed color-mapped (index) image
9 run-length encoded color-mapped (index) image
------------------------------------------------------------------------------*/


// Load An Uncompressed File
GLuint loadUncompressedTGA(const char* path, FILE* file, unsigned char* header);
// Load A Compressed File
GLuint loadCompressedTGA(const char* path, FILE* file, unsigned char* header);


#endif
