#ifndef imageloader_hpp
#define imageloader_hpp

#include "stb_image.h"

/*----------------------------------------------------------------------------*/
unsigned int loadTexture(char const * path);
/*------------------------------------------------------------------------------
< Load Texture >

Load data from the image
Generating GL_TEXTURE_2D (GL_RED / GL_RGB / GL_RGBA)
................................................................................
Return:

GLuint textureID
................................................................................
Example:

unsigned int texture = loadTexture("path/image.jpg");
------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
unsigned int loadCubemap(std::vector<std::string> faces);
/*------------------------------------------------------------------------------
< Load Texture >

Load data from the image
Generating GL_TEXTURE_CUBE_MAP (GL_RGB)
................................................................................
Return:

GLuint textureID
................................................................................
Example:

std::vector<std::string> faces {
	"path/right.jpg",
	"path/left.jpg",
	"path/top.jpg",
	"path/bottom.jpg",
	"path/back.jpg",
	"path/front.jpg"
};
unsigned int cubemapTexture = loadCubemap(faces);
------------------------------------------------------------------------------*/


#endif /* imageloader_hpp */
