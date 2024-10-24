// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "custom_controls.hpp"

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



void c_computeMatricesFromInputs(){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, 1024/2, 768/2);

	// Compute new orientation
	c_horizontalAngle += c_mousec_speed * float(1024/2 - xpos );
	c_verticalAngle   += c_mousec_speed * float( 768/2 - ypos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(c_verticalAngle) * sin(c_horizontalAngle), 
		sin(c_verticalAngle),
		cos(c_verticalAngle) * cos(c_horizontalAngle)
	);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(c_horizontalAngle - 3.14f/2.0f), 
		0,
		cos(c_horizontalAngle - 3.14f/2.0f)
	);
	
	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	// Move forward
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		c_position += direction * deltaTime * c_speed;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		c_position -= direction * deltaTime * c_speed;
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		c_position += right * deltaTime * c_speed;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		c_position -= right * deltaTime * c_speed;
	}

    if (glfwGetKey( window, GLFW_KEY_L ) == GLFW_PRESS)
    {
        if (LightMode)
            LightMode = 0;
        else
            LightMode = 1;
    }

	float FoV = c_initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	c_ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	c_ViewMatrix       = glm::lookAt(
								c_position,           // Camera is here
								c_position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}