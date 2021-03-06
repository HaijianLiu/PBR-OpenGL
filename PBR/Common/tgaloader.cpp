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
	printf("[loadTGA] Loading TGA file... <%s>\n",path); // Debug Information

	// File Header To Determine File Type
	unsigned char header[18];

	FILE* file;              // Declare File Pointer
	file = fopen(path,"rb"); // Open File For Reading

	// Attempt To Open The File Header
	if (file == NULL) {
		printf("[ERROR] Impossible to open.\n"); // Debug Information
		return 0;
	}
	// Attempt To Read The File Header
	if (fread(&header,sizeof(header),1,file) == 0) {
		printf("[ERROR] Impossible to read\n"); // Debug Information
		return 0;
	}

	// Debug Information output first 18 bytes header
	printf("Header: [ ");
	for (int i = 0; i < 18; i++) {
		printf("%d ", header[i]);
	}
	printf("]\n");

	// If The File Header Matches The Uncompressed Header
	if (header[2] >= 1 && header[2] <= 3) {
		// Load An Uncompressed TGA
		printf("Uncompressed "); // Debug Information
		return loadUncompressedTGA(path,file,header);
	} else
	// If The File Header Matches The Compressed Header
	if (header[2] >= 9 && header[2] <= 11) {
		// Load A Compressed TGA
		printf("Compressed "); // Debug Information
		return loadRLEcompressedTGA(path,file,header);
	} else {
		// If It Doesn't Match Either One
		printf("[ERROR] Not a right TGA file.\n"); // Debug Information
		return 0;
	}
}

// Load An Uncompressed TGA!
GLuint loadUncompressedTGA(const char* path, FILE* file, unsigned char* header) {

	// imageSize: Amount Of Memory Needed To Hold The Image
	unsigned int width, height, bpp, type = 0, imageSize;
	// Actual image data
	unsigned char * data;

	bpp = header[16]; // Calculate Bits Per Pixel

	// Check for Image type (field 3)
	// 1 uncompressed color-mapped image
	if (header[2] == 1) {
		printf("color-mapped.\n"); // Debug Information
		printf("[ERROR] Unupported color-mapped (index) image.\n"); // Debug Information
		return 0;               // If Not, Return False
	}
	// 2 uncompressed true-color image
	if (header[2] == 2) {
		printf("true-color "); // Debug Information
		// RGB or RGBA channel
		if (bpp == 24) {
			printf("RGB ");
			type = 0;
		} else {
			printf("RGBA ");
			type = 1;
		}
	}
	// 3 uncompressed black-and-white (grayscale) image
	if (header[2] == 3) {
		printf("black-and-white (grayscale) "); // Debug Information
		type = 2;
	}

	// Calculate image size
	width  = header[13] * 256 + header[12];
	height = header[15] * 256 + header[14];
	// Calculate Memory Needed To Store Image
	imageSize = bpp/8 * width * height;
	printf("< %d X %d >.\n",width,height);

	// Allocate Memory : Create a buffer
	data = new unsigned char [imageSize];
	if (data == NULL) {           // Make Sure It Was Allocated Ok
		printf("[ERROR] Can't Allocate Memory.\n"); // Debug Information
		return 0;               // If Not, Return False
	}
	// Attempt To Read All The Image Data
	if (fread(data,1,imageSize,file) != imageSize) {
		printf("[ERROR] Can't read the image.\n"); // Debug Information
		return 0;
	}
	// Everything is in memory, closed the file
	fclose (file);
	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1,&textureID);
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D,textureID);
	// Give the image to OpenGL
	switch (type) {
		case 0:
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_BGR,GL_UNSIGNED_BYTE,data);
		break;
		case 1:
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_BGRA,GL_UNSIGNED_BYTE,data);
		break;
		case 2:
		glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,width,height,0,GL_DEPTH_COMPONENT,GL_UNSIGNED_BYTE,data);
		break;
	}
	// delete image data
	delete [] data;
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Return the ID of the texture we just created
	return textureID;
}

GLuint loadRLEcompressedTGA(const char* path, FILE* file, unsigned char* header){
	// imageSize: Amount Of Memory Needed To Hold The Image
	unsigned int width, height, bpp, type = 0, imageSize;
	// Actual image data
	unsigned char * data;

	bpp = header[16]; // Calculate Bits Per Pixel

	// Check for Image type (field 3)
	// 1 uncompressed color-mapped image
	if (header[2] == 1) {
		printf("color-mapped.\n"); // Debug Information
		printf("[ERROR] Unupported color-mapped (index) image.\n"); // Debug Information
		return 0;               // If Not, Return False
	}
	// 2 uncompressed true-color image
	if (header[2] == 10) {
		printf("true-color "); // Debug Information
		// RGB or RGBA channel
		if (bpp == 24) {
			printf("RGB ");
			type = 0;
		} else {
			printf("RGBA ");
			type = 1;
		}
	}
	// 3 uncompressed black-and-white (grayscale) image
	if (header[2] == 11) {
		printf("black-and-white (grayscale) "); // Debug Information
		type = 2;
	}

	// Calculate image size
	width  = header[13] * 256 + header[12];
	height = header[15] * 256 + header[14];
	// Calculate Memory Needed To Store Image
	imageSize = bpp/8 * width * height;
	printf("< %d X %d >.\n",width,height);

	// Allocate Memory : Create a buffer
	data = new unsigned char [imageSize];
	if (data == NULL) {       // Make Sure It Was Allocated Ok
		printf("[ERROR] Can't Allocate Memory.\n"); // Debug Information
		return 0;               // If Not, Return False
	}

	unsigned int pixelCount = width * height; // Number Of Pixels In The Image
	unsigned int currentPixel = 0;            // Current Pixel We Are Reading From Data
	unsigned int currentByte  = 0;            // Current Byte We Are Writing Into Imagedata
	unsigned int bytePerPixel = bpp/8;
	// Storage For 1 Pixel
	unsigned char* colorBuffer = new unsigned char [bytePerPixel] ;

	// Read TGA file
	do {
		unsigned int chunkHeader = 0; // Variable To Store The Value Of The Id Chunk
		if (fread(&chunkHeader,1,1,file) == 0) { // Attempt To Read The Chunk's Header
			printf("[ERROR] Can't Read The Chunk's Header.\n"); // Debug Information
			return 0; // If It Fails, Return False
		}
		// Debug Information for chunkHeader and currentPixel// std::cout << chunkHeader << "[" << currentPixel << "]" << pixelCount << '\n';
		if (chunkHeader < 128) { // If The Chunk Is A RAW Chunk
			chunkHeader++; // Add 1 To The Value To Get Total Number Of Raw Pixels
			// Repetition Count specifies how many pixel values are actually contained in the next field
			// Start Pixel Reading Loop
			for (int i = 0; i < chunkHeader; i++) {
				// Try To Read 1 Pixel
				if (fread(colorBuffer,1,bytePerPixel,file) == 0) {
					printf("[ERROR] Can't Read The Pixels (RAW Chunk).\n"); // Debug Information
					return 0;
				}
				// Dubug Information : the value of colorBuffer // printf("%d\n",colorBuffer[0]); // printf("%d\n",colorBuffer[1]); // printf("%d\n",colorBuffer[2]);
				// Take the color values stored in colorbuffer and writing them to the imageData variable to be used later
				data[currentByte] = colorBuffer[0];   // Write The 'B' Byte
				data[currentByte+1] = colorBuffer[1]; // Write The 'G' Byte
				data[currentByte+2] = colorBuffer[2]; // Write The 'R' Byte
				if(bytePerPixel == 4) { // If It's A 32bpp Image...
					data[currentByte+3] = colorBuffer[3]; // Write The 'A' Byte
				}
				// Increment The Byte Counter By The Number Of Bytes In A Pixel
				currentByte += bytePerPixel;
				currentPixel++; // Increment The Number Of Pixels By 1
			}
		} else { // If It's An RLE Header. Subtract 127 from the chunkheader to get the amount of times the next color is repeated
			// this count indicates how many successive pixels have the pixel value specified by the Pixel Value field
			chunkHeader -= 127; // Subtract 127 To Get Rid Of The ID Bit
			// Try To Read 1 Pixel
			if (fread(colorBuffer,1,bytePerPixel,file) == 0) {
				printf("[ERROR] Can't Read The Pixels (RLE Chunk).\n"); // Debug Information
				return 0;
			}
			// Dubug Information : the value of colorBuffer // printf("%d\n",colorBuffer[0]); // printf("%d\n",colorBuffer[1]); // printf("%d\n",colorBuffer[2]);
			// Start Pixel Reading Loop
			for (int i = 0; i < chunkHeader; i++) {
				// Take the color values stored in colorbuffer and writing them to the imageData variable to be used later
				data[currentByte] = colorBuffer[0];   // Write The 'B' Byte
				data[currentByte+1] = colorBuffer[1]; // Write The 'G' Byte
				data[currentByte+2] = colorBuffer[2]; // Write The 'R' Byte
				if(bytePerPixel == 4) { // If It's A 32bpp Image...
					data[currentByte+3] = colorBuffer[3]; // Write The 'A' Byte
				}
				// Increment The Byte Counter By The Number Of Bytes In A Pixel
				currentByte += bytePerPixel;
				currentPixel++; // Increment The Number Of Pixels By 1
			}
		}
	} while(currentPixel < pixelCount); // More Pixels To Read? ... Start Loop Over
	// Everything is in memory, closed the file
	fclose (file);
	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1,&textureID);
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D,textureID);
	// Give the image to OpenGL
	switch (type) {
		case 0:
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_BGR,GL_UNSIGNED_BYTE,data);
		break;
		case 1:
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_BGRA,GL_UNSIGNED_BYTE,data);
		break;
		case 2:
		glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,width,height,0,GL_DEPTH_COMPONENT,GL_UNSIGNED_BYTE,data);
		break;
	}
	// delete image data
	delete [] data;
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Return the ID of the texture we just created
	return textureID;
}
