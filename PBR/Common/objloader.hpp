#ifndef objloader_hpp
#define objloader_hpp

bool loadObj(const char * path,
	std::vector<glm::vec3> & outVertices,
	std::vector<glm::vec2> & outUVs,
	std::vector<glm::vec3> & outNormals);

#endif /* objloader_hpp */
