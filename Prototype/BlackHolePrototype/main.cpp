
// base surrounding code for opengl project based off of
// tutorial by Victor Gordan on youtube "OpenGL Tutorial"

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/ext.hpp>
#include<glm/gtc/matrix_transform.hpp>


#include "Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"


// Global constants for window width and height
const unsigned int width = 800;
const unsigned int height = 800;


// Manually make vertices for rendering a pyramid
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
};

// Set index buffer to write order that OpenGL should write vertices in
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};

int main() {

	// Init glfw
	glfwInit();

	// Tell glfw about itself so it knows what version we are using
	// We are using OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// We are using the CORE profile, which is not backwards compatible
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);






	// Create window
	GLFWwindow* window = glfwCreateWindow(width, height, "BlackHolePrototype", NULL, NULL);
	if (window == NULL) {

		std::cout << "Error: Failed to create GLFW window" << std::endl;
		return -1;

	}
	// Now we have a window, we must tell glfw to use it (very silly)
	glfwMakeContextCurrent(window);


	// Load GLAD
	gladLoadGL();

	// Specify viewport of OpenGL in the window
	glViewport(0, 0, width, height);

	// Generate shader program obj using Vert/Frag shader pipeline
	Shader shaderProgram("default.vert", "default.frag");


	//--------------------------------------------------------------------------------------
	//---------------------------------------VBOS and stuff---------------------------------
	//--------------------------------------------------------------------------------------

	// Generate VAO object and bind it
	VAO VAO1;
	VAO1.Bind();

	// Generate VBO and link to given vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generate EBO and link to given indices
	EBO EBO1(indices, sizeof(indices));

	// Link VAO1 to VBO1
	// COORDINATES
	VAO1.LinkAttributes(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	// COLOURS
	VAO1.LinkAttributes(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3*sizeof(float)));
	// TEXTURES
	VAO1.LinkAttributes(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind all Objs
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


	// Get uniforms ready to be sent to .vert shader program
	GLuint uniformID = glGetUniformLocation(shaderProgram.ID, "scale");


	// Texture stuff
	Texture tiles("tile_floor.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	tiles.textureUnit(shaderProgram, "tex0", 0);

	// Timer for animation
	float rotation = 0.f;
	double prevTime = glfwGetTime();


	// Make 3d triangels draw in correct order upon rotation
	glEnable(GL_DEPTH_TEST);

	//--------------------------------------------------------------------------------------
	//---------------------------------------MAIN LOOP--------------------------------------
	//--------------------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window)) {

		glClearColor(0.35f, 0.17f, 0.65f, 1.f);
		// Clear colour buffer and depth buffer to be rewritten next frame swap
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL to use this shader program
		shaderProgram.Activate();


		// Stuff for 3d ---------------------------------------------------
		// Animation

		// Simple timer to rotate pyramid
		double currentTime = glfwGetTime();
		if (currentTime - prevTime >= 1 / 60) {
			rotation += 0.5f;
			prevTime = currentTime;
		}
		

		// Initialise matrices for 3d implemenmtation
		glm::mat4 model = glm::mat4(1.f);
		glm::mat4 view = glm::mat4(1.f);
		glm::mat4 projection = glm::mat4(1.f);
		
		// Edit model matrix so it does stuff
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.f, 1.f, 0.f));
		// Change view so that the camera is in a point where we can see
		view = glm::translate(view, glm::vec3(0.0f, -2.5f, -15.f));
		// Set how data is seen by the camera, with FOV, Aspect ratio, closest sm can be, and farthest sm can be
		projection = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 100.0f);

		// Send these matrices into shader program
		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		int projectionLoc = glGetUniformLocation(shaderProgram.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));



		// -----------------------------------------------------------------

		// Send uniforms to .vert shader
		glUniform1f(uniformID, 0.5f);
		// Bind texture object
		tiles.Bind();

		// Bind the created VAO so OpenGL will use it
		VAO1.Bind();
		// Draw triangle using GL_TRIANGLES primitive
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);

		// Tell glfw to process all pooled events
		glfwPollEvents();

	}



	//--------------------------------------------------------------------------------------
	//---------------------------------------CLEANUP----------------------------------------
	//--------------------------------------------------------------------------------------

	// Delete all array and buffer objects
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();

	// Delete textures
	tiles.Delete();

	// Delete shader program
	shaderProgram.Delete();

	// Destroy window
	glfwDestroyWindow(window);
	// End glfw
	glfwTerminate();
	return 0;
}