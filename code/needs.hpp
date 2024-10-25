/*
Author: Adarsh Thirugnansambandam Sriuma
Class: ECE6122 (A)
Last Date Modified: Oct 24, 2024
Description:
Header file for objDetails
*/

#ifndef NEEDS_HPP_
#define NEEDS_HPP_

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>

struct objDetails {
	std::string name;
	glm::mat4 ModelMatrix;
	GLuint VAO_ID;
	GLsizei indices_size;
	GLuint textureID;
    int width;
    int height;
    unsigned char* data;
    std::string use_texture_format;
};

#endif