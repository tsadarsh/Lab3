// Include standard headers
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
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>


std::vector<GLuint> textures; 
GLuint textureID;
GLuint textureID2;
std::vector<GLuint> VAOs;
std::vector<GLuint> TEXTURE_UNITS;
std::vector<glm::mat4> model_matrices;
std::vector<GLsizei> indices_size;

int main( void )
{
	// Initialize GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make macOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Tutorial 09 - Loading with AssImp", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
	glEnable(GL_CULL_FACE);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "shaders/StandardShading.vertexshader", "shaders/StandardShading.fragmentshader" );

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

	GLuint tID;

	glActiveTexture(GL_TEXTURE0);
	tID = c_loadTexture("assets/textures/12951_Stone_Chess_Board_diff_flipped.jpg");
	textures.push_back(tID);
	TEXTURE_UNITS.push_back(GL_TEXTURE0);

	glActiveTexture(GL_TEXTURE1);
	tID = c_loadTexture("assets/textures/uvmap.jpg");
	textures.push_back(tID);
	TEXTURE_UNITS.push_back(GL_TEXTURE1);

	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

	// Read our .obj file
	GLuint VAO_ID;
	GLsizei IDX_size;
	glm::mat4 ModelMatrix;

	VAO_ID = c_loadOBJ("assets/geometry/chess-board.obj", 0, IDX_size);
	VAOs.push_back(VAO_ID);
	ModelMatrix = glm::mat4(1.0);
	model_matrices.push_back(ModelMatrix);
	indices_size.push_back(IDX_size);
	
	for (int i = 0; i < 12; i++) 
	{
		VAO_ID = c_loadOBJ("assets/geometry/new-pieces.obj", i, IDX_size);
		VAOs.push_back(VAO_ID);
		ModelMatrix = glm::mat4(1.0);
		model_matrices.push_back(ModelMatrix);
		indices_size.push_back(IDX_size);
	}

	// Get a handle for our "LightPosition" uniform
	glUseProgram(programID);
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

	// For speed computation
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	glm::mat4 MVP;
	do{

		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;
		if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1sec ago
			// printf and reset
			printf("%f ms/frame\n", 1000.0/double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
		glm::vec3 lightPos = glm::vec3(4,4,4);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		for (int iObj = 0; iObj < VAOs.size(); iObj++)
		{
			ModelMatrix = model_matrices[iObj];
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			glActiveTexture(TEXTURE_UNITS[iObj]);
			glUniform1i(TextureID, iObj); // do not use textures[iObj]
			glBindVertexArray(VAOs[iObj]);
			glDrawElements(GL_TRIANGLES, indices_size[iObj], GL_UNSIGNED_SHORT, (void*)0);

			glBindVertexArray(0);
		}
		// glm::mat4 ModelMatrix = glm::mat4(1.0);
		// glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// // Send our transformation to the currently bound shader, 
		// // in the "MVP" uniform
		// glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		// glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		// glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

		// glm::vec3 lightPos = glm::vec3(4,4,4);
		// glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		// // Bind our texture in Texture Unit 0
		// glActiveTexture(GL_TEXTURE0);
		// // glBindTexture(GL_TEXTURE_2D, textureID);
		// // Set our "myTextureSampler" sampler to use Texture Unit 0
		// glUniform1i(TextureID, 0);

		// glBindVertexArray(VertexArrayID);
		// //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

		// // Draw the triangles !
		// glDrawElements(
		// 	GL_TRIANGLES,      // mode
		// 	indices.size(),    // count
		// 	GL_UNSIGNED_SHORT,   // type
		// 	(void*)0           // element array buffer offset
		// );

		// glBindVertexArray(0);

		// glBindVertexArray(VertexArrayID2);
		// glActiveTexture(GL_TEXTURE1);
		// glUniform1i(TextureID, 1);
		// // glBindTexture(GL_TEXTURE_2D, textureID2);
		// glm::mat4 ModelMatrix2 = glm::mat4(1.0);
		// ModelMatrix2 = glm::translate(ModelMatrix2, glm::vec3(2.0f, 0.0f, 0.0f));
		// glm::mat4 MVP2 = ProjectionMatrix * ViewMatrix * ModelMatrix2;

		// // Send our transformation to the currently bound shader, 
		// // in the "MVP" uniform
		// glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]);
		// glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix2[0][0]);
		
		// // Draw the triangles !
		// glDrawElements(
		// 	GL_TRIANGLES,      // mode
		// 	indices2.size(),    // count
		// 	GL_UNSIGNED_SHORT,   // type
		// 	(void*)0           // element array buffer offset
		// );

		// glBindVertexArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	// glDeleteBuffers(1, &vertexbuffer);
	// glDeleteBuffers(1, &uvbuffer);
	// glDeleteBuffers(1, &normalbuffer);
	// glDeleteBuffers(1, &elementbuffer);
	glDeleteProgram(programID);
	// glDeleteTextures(1, &Texture);
	// glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

