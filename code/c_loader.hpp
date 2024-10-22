#ifndef C_LOADER_H_
#define C_LOADER_H_

#include <vector>
#include <string>
#include <glm/glm.hpp>

void c_loadOBJ(std::string path, 
    std::vector<unsigned short>&,
	std::vector<glm::vec3>&,
	std::vector<glm::vec2>&,
	std::vector<glm::vec3>&,
	int);

#endif