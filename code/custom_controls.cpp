/*
Author: Adarsh Thirugnansambandam Sriuma
Class: ECE6122 (A)
Last Date Modified: Oct 24, 2024
Description:
Custom implementation for keyboard controls
*/

// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "custom_controls.hpp"
#include <iostream>

glm::mat4 c_ViewMatrix;
glm::mat4 c_ProjectionMatrix;
int LightMode = 1;

glm::mat4 c_getViewMatrix(){
	return c_ViewMatrix;
}
glm::mat4 c_getProjectionMatrix(){
	return c_ProjectionMatrix;
}
int c_getLightStatus(){
    return LightMode;
}


// Initial position : on +Z
glm::vec3 c_position = glm::vec3( 0, 0, 5 ); 
// Initial horizontal angle : toward -Z
float c_horizontalAngle = 3.14f;
// Initial vertical angle : none
float c_verticalAngle = 0.0f;
// Initial Field of View
float c_initialFoV = 45.0f;

float c_speed = 3.0f; // 3 units / second
float c_mousec_speed = 0.005f;


glm::vec3 p = glm::vec3(5, 0, 1.00);
glm::vec3 move_speed = glm::vec3(1, 0, 0);
glm::vec3 yaw_speed = glm::vec3(0, 1, 0);
glm::vec3 pitch_speed = glm::vec3(0, 0, 1);

void c_computeMatricesFromInputs(){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(c_verticalAngle) * sin(c_horizontalAngle), 
		sin(c_verticalAngle),
		cos(c_verticalAngle) * cos(c_horizontalAngle)
	);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(p[1]), 
		0,
		0
	);
	
	// Up vector
	// glm::vec3 up = glm::rotate(glm::cross(p, glm::vec3(0, 1, 0)), -1.57);

	// Move forward
	if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
		p -= move_speed * deltaTime * c_speed;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
		p += move_speed * deltaTime * c_speed;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
		p -= yaw_speed * deltaTime * c_speed;
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
		p += yaw_speed * deltaTime * c_speed;
	}
	// Strafe up
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		p += pitch_speed * deltaTime * c_speed;
	}
	// Strafe down
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		p -= pitch_speed * deltaTime * c_speed;
	}
    if (glfwGetKey( window, GLFW_KEY_L ) == GLFW_PRESS)
    {
        if (LightMode)
            LightMode = 0;
        else
            LightMode = 1;
    }
	glm::vec3 positionInCartesian = glm::vec3 {
		p[0] * cos(p[2]) * sin(p[1]),
		p[0] * sin(p[2]),
		p[0] * cos(p[1]) * cos(p[2]),
	};

	float FoV = c_initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	c_ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	c_ViewMatrix       = glm::lookAt(
								positionInCartesian,           // Camera is here
								glm::vec3(0,0,0), // and looks here : at the same position, plus "direction"
								glm::vec3(0, 1, 0)                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
	// std::cout << p[1]  << " " << p[2] << std::endl;
}