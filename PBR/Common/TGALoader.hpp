#ifndef tagloader_hpp
#define tagloader_hpp

// typedef struct{
// 	GLubyte* imageData;         // Hold All The Color Values For The Image.
// 	GLuint width;               // The Width Of The Entire Image.
// 	GLuint height;              // The Height Of The Entire Image.
// 	GLuint bpp;                 // Hold The Number Of Bits Per Pixel.
// 	GLuint type;                // Data Stored In * ImageData (GL_RGB Or GL_RGBA)
// 	GLuint texID;               // Texture ID For Use With glBindTexture.
// }Texture;
//
// typedef struct{
// 	GLubyte header[6];          // Holds The First 6 Useful Bytes Of The File
// 	GLuint bytesPerPixel;       // Number Of BYTES Per Pixel (3 Or 4)
// 	GLuint imageSize;           // Amount Of Memory Needed To Hold The Image
// 	GLuint bpp;                 // Number Of BITS Per Pixel (24 Or 32)
// 	GLuint width;               // Width Of Image
// 	GLuint height;              // Height Of Image
// 	GLuint type;                // The Type Of Image, GL_RGB Or GL_RGBA
// }TGA;

// 	GLubyte* data; // Hold All The Color Values For The Image.
// 	GLuint width;  // The Width Of The Entire Image.
// 	GLuint height; // The Height Of The Entire Image.


// Load A TGA File!
GLuint loadTGA(const char* path);
// Load An Uncompressed File
GLuint loadUncompressedTGA(const char* path, FILE* file);
// Load A Compressed File
GLuint loadCompressedTGA(const char* path, FILE* file);

#endif
