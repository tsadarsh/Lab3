/*
Author: Adarsh Thirugnansambandam Sriuma
Class: ECE6122 (A)
Last Date Modified: Oct 24, 2024
Description:
Header file
*/

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;


#include "c_loader.hpp"

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include "custom_controls.hpp"

#include <assimp/postprocess.h>     // Post processing flags