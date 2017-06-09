#ifndef objloader_hpp
#define objloader_hpp

bool LoadOBJ(
	const char * path,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
);

#endif /* objloader_hpp */
