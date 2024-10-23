#ifndef C_LOADER_H_
#define C_LOADER_H_

#include <vector>
#include <string>
#include <glm/glm.hpp>

GLuint c_loadOBJ(std::string filePath, int meshIndex, GLsizei& index_size, std::vector<GLuint>& texturesIds, unsigned int pp_flags);

// GLuint c_loadTexture (std::string);

#endif