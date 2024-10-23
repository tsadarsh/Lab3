#ifndef CUSTOM_OBJLOADER_HPP_
#define CUSTOM_OBJLOADER_HPP_

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

bool c_loadAssImp(
	const char *, 
	std::vector<unsigned short> &,
	std::vector<glm::vec3> &,
	std::vector<glm::vec2> &,
	std::vector<glm::vec3> &,
    int meshIndex,
	std::vector<GLuint>& texturesIds,
	unsigned int pp_flags
);

void loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, std::vector<GLuint>& texturesIds);

#endif