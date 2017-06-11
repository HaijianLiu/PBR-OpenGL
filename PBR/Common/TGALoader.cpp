// Include standard headers
#include <iostream>

// Include GLEW (include before gl.h and glfw.h)
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>

// Include header file
#include "tgaloader.hpp"


GLuint loadTGA(const char* path){

	// Set path
	std::string currentDir = __FILE__;
	std::string targetDir = currentDir.substr(0,currentDir.rfind("/"));
	targetDir = targetDir.substr(0,targetDir.rfind("/")) + "/Texture/";
	path = targetDir.append(path).c_str();
	printf("loadTGA: Loading TGA file %s...\n",path);

	// File Header To Determine File Type
	GLubyte headerTGA[12];
	GLubyte uTGAcompare[12] = {0,0, 2,0,0,0,0,0,0,0,0,0}; // Uncompressed TGA Header
	GLubyte cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0}; // Compressed TGA Header

	FILE* file;              // Declare File Pointer
	file = fopen(path,"rb"); // Open File For Reading

	// Attempt To Open The File Header
	if(file == NULL){
		printf("loadTGA: Impossible to open.\n"); // Debug Information
		return 0;
	}
	// Attempt To Read The File Header
	if(fread(&headerTGA,sizeof(headerTGA),1,file) == 0){
		printf("loadTGA: Impossible to read\n"); // Debug Information
		return 0;
	}

	// If The File Header Matches The Uncompressed Header
	if(memcmp(uTGAcompare,&headerTGA,sizeof(headerTGA)) == 0){
		// Load An Uncompressed TGA
		printf("loadTGA: Load An Uncompressed TGA.\n"); // Debug Information
		return loadUncompressedTGA(path,file);
	}else
	// If The File Header Matches The Compressed Header
	if(memcmp(cTGAcompare,&headerTGA,sizeof(headerTGA)) == 0){
		// Load A Compressed TGA
		printf("loadTGA: Load A Compressed TGA.\n"); // Debug Information
		return loadCompressedTGA(path,file);
	}else{
		// If It Doesn't Match Either One
		printf("loadTGA: Not a right TGA file.\n"); // Debug Information
		return 0;
	}
}

// Load An Uncompressed TGA!
GLuint loadUncompressedTGA(const char* path, FILE* file){

	unsigned char header[6];
	unsigned int imageSize,width,height,bpp;
	// Actual RGB data
	unsigned char * data;

	// GLubyte header[6];          // Holds The First 6 Useful Bytes Of The File
	// GLuint bpp;
	// GLuint type;
	// GLuint imageSize;           // Amount Of Memory Needed To Hold The Image

	// Attempt To Read Next 6 Bytes
	if(fread(&header,sizeof(header),1,file) == 0){
		printf("loadTGA: Can't read the Uncompressed TGA\n"); // Debug Information
		return 0;
	}

	width  = header[1] * 256 + header[0];   // Calculate Height
	height = header[3] * 256 + header[2];   // Calculate The Width
	bpp = header[4];                            // Calculate Bits Per Pixel

	// Make Sure All Information Is Valid
	if((width <= 0) || (height <= 0) || ((bpp != 24) && (bpp !=32))){
		printf("loadTGA: Can't read the Uncompressed TGA (header).\n"); // Debug Information
		return 0;
	}

	if(bpp == 24){    // Is It A 24bpp Image?
		printf("loadTGA: GL_RGB file.\n"); // Debug Information
	}else{                     // If It's Not 24, It Must Be 32
		printf("loadTGA: GL_RGBA file.\n"); // Debug Information
	}

	imageSize = bpp/8 * width * height; // Calculate Memory Needed To Store Image

	// Allocate Memory : Create a buffer
	data = new unsigned char [imageSize];
	if(data == NULL){           // Make Sure It Was Allocated Ok
		printf("loadTGA: Can't Allocate the Uncompressed TGA.\n"); // Debug Information
		return 0;               // If Not, Return False
	}

	// Attempt To Read All The Image Data
	if(fread(data,1,imageSize,file) != imageSize){
		printf("loadTGA: Can't read the Uncompressed TGA (image).\n"); // Debug Information
		return 0;
	}
	// Everything is in memory now, the file wan be closed
	fclose (file);
	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
	// OpenGL has now copied the data. Free our own version
	delete [] data;

	// ... nice trilinear filtering.
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Return the ID of the texture we just created
	return textureID;
}

GLuint loadCompressedTGA(const char* path, FILE* file){
	return 0;
}
