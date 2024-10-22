#ifndef CUSTOM_OBJLOADER_HPP_
#define CUSTOM_OBJLOADER_HPP_

#include <vector>
#include <glm/glm.hpp>

bool c_loadAssImp(
	const char *, 
	std::vector<unsigned short> &,
	std::vector<glm::vec3> &,
	std::vector<glm::vec2> &,
	std::vector<glm::vec3> &,
    int meshIndex
);

#endif