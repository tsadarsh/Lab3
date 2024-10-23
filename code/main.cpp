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

#include <assimp/postprocess.h>     // Post processing flags


std::vector<GLuint> texturesIds; // #TODO: couple with objDetails

std::vector<objDetails> allObjects;

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

	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

	// Read our .obj file
	GLuint VAO_ID;
	GLsizei IDX_size;
	glm::mat4 ModelMatrix;

	objDetails chessBoardObj;
	chessBoardObj.use_texture_format = "jpg";
	VAO_ID = c_loadOBJ("assets/geometry/chess-board.obj", 0, IDX_size, chessBoardObj, 0);
	chessBoardObj.VAO_ID = VAO_ID;
	glm::mat4 myScalingMatrix = glm::scale(glm::mat4(1), glm::vec3(1,1,1));
	chessBoardObj.ModelMatrix = myScalingMatrix;
	chessBoardObj.indices_size = IDX_size;
	// chessBoardObj.textureID = texturesIds.back();
	allObjects.push_back(chessBoardObj);
	
	for (int i = 0; i < 12; i++) 
	{
		objDetails chessPieceObj;
		chessPieceObj.use_texture_format = "bmp";
		VAO_ID = c_loadOBJ("assets/geometry/new-pieces.obj", i, IDX_size, chessPieceObj, 0);
		chessPieceObj.VAO_ID = VAO_ID;
		chessPieceObj.ModelMatrix = glm::mat4(1.0);
		chessPieceObj.indices_size = IDX_size;
		// chessPieceObj.textureID = texturesIds.back();
		allObjects.push_back(chessPieceObj);
	}

	// Translating each piece by eyeballing
	for (int i=0; i < 7; i++)
	{
		objDetails w_pawn = allObjects[1];
		w_pawn.ModelMatrix = glm::translate(glm::mat4(), glm::vec3(0.125f*(i+1), 0.0f, 0.0f));
		allObjects.push_back(w_pawn);
	}
	objDetails w_rook = allObjects[2];
	w_rook.ModelMatrix = glm::translate(glm::mat4(), glm::vec3(0.89f, 0.0f, 0.0f));
	allObjects.push_back(w_rook);

	objDetails w_knight = allObjects[3];
	w_knight.ModelMatrix = glm::translate(glm::mat4(), glm::vec3(0.63f, 0.0f, 0.0f));
	allObjects.push_back(w_knight);
	
	objDetails w_bishop = allObjects[4];
	w_bishop.ModelMatrix = glm::translate(glm::mat4(), glm::vec3(0.38f, 0.0f, 0.0f));
	allObjects.push_back(w_bishop);
	
	for (int i=0; i < 7; i++)
	{
		objDetails b_pawn = allObjects[7];
		b_pawn.ModelMatrix = glm::translate(glm::mat4(), glm::vec3(0.125f*(i+1), 0.0f, 0.0f));
		allObjects.push_back(b_pawn);
	}
	objDetails b_rook = allObjects[8];
	b_rook.ModelMatrix = glm::translate(glm::mat4(), glm::vec3(0.89f, 0.0f, 0.0f));
	allObjects.push_back(b_rook);

	objDetails b_knight = allObjects[9];
	b_knight.ModelMatrix = glm::translate(glm::mat4(), glm::vec3(0.63f, 0.0f, 0.0f));
	allObjects.push_back(b_knight);
	
	objDetails b_bishop = allObjects[10];
	b_bishop.ModelMatrix = glm::translate(glm::mat4(), glm::vec3(0.38f, 0.0f, 0.0f));
	allObjects.push_back(b_bishop);

	std::cout << "Got these Texture IDs: " << std::endl;
	for (int i=0; i < texturesIds.size(); i++)
	{
		std::cout << texturesIds[i] << std::endl;
	}

	// Get a handle for our "LightPosition" uniform
	glUseProgram(programID);
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
	glUniform1i(TextureID, 0);

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

		for (int iObj = 0; iObj < allObjects.size(); iObj++)
		{
			ModelMatrix = allObjects[iObj].ModelMatrix;
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			glBindTexture(GL_TEXTURE_2D, allObjects[iObj].textureID);
			glBindVertexArray(allObjects[iObj].VAO_ID);
			
			glDrawElements(GL_TRIANGLES, allObjects[iObj].indices_size, GL_UNSIGNED_SHORT, (void*)0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glBindVertexArray(0);
		}

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

