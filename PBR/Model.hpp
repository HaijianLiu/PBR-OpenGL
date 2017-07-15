#ifndef Model_hpp
#define Model_hpp

// Include stbi Library
#include "stb_image.h"
// Include assimp Library
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Include Class
#include "Mesh.hpp"

class Model {
public:
	/*  Functions   */
	// constructor, expects a filepath to a 3D model.
	Model(std::string const& path);
	// if the model has no defined textures, load textures manually
	void loadPBRTextures(const char* diffusePath, const char* normalPath, const char* metalPath, const char* roughPath, const char* aoPath);
	void loadPBRTextures(const char* diffusePath, const char* normalPath, const char* roughPath, const char* aoPath);
	// draws the model, and thus all its meshes
	void Draw(Shader shader);

private:
	/*  Model Data */
	std::vector<Texture> texturesLoaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	std::vector<Mesh> meshes;
	std::string directory;

	/*  Functions   */
	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void loadModel(std::string const& path);
	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	// checks all material textures of a given type and loads the textures if they're not loaded yet.
	// the required info is returned as a Texture struct.
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

unsigned int loadTexture(const char* path, const std::string& directory);

#endif /* Model_hpp */
