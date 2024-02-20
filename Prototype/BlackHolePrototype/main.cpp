
// base surrounding code for opengl project based off of
// tutorial by Victor Gordan on youtube "OpenGL Tutorial"

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"


// manually make vertices for rendering a triangle
GLfloat vertices[] = {
	-0.5f, -0.5f * float(sqrt(3)) / 3, 0.f, //no z because 2d atm
	0.5f, -0.5f * float(sqrt(3)) / 3, 0.f,
	0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.f,

	//vertices for inner triangle
	-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.f,
	0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.f,
	0.f, -0.5f * float(sqrt(3)) / 3, 0.f
};

// Set index buffer to write order that OpenGL should write vertices in
GLuint indices[] = {
	0, 3, 5, // lower left triangle
	3, 2, 4, // lower right triangle
	5, 4, 1 // upper triangle
};

int main() {

	// init glfw
	glfwInit();

	// tell glfw about itself so it knows what version we are using
	// we are using OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// We are using the CORE profile, which is not backwards compatible
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);






	// create window
	GLFWwindow* window = glfwCreateWindow(800, 800, "BlackHolePrototype", NULL, NULL);
	if (window == NULL) {

		std::cout << "Error: Failed to create GLFW window" << std::endl;
		return -1;

	}
	// now we have a window, we must tell glfw to use it (very silly)
	glfwMakeContextCurrent(window);


	// load GLAD
	gladLoadGL();

	// specify viewport of OpenGL in the window
	glViewport(0, 0, 800, 800);

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
	VAO1.LinkVBO(VBO1, 0);
	// Unbind all Objs
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();



	// Background colour of window
	glClearColor(0.35f, 0.17f, 0.65f, 1.f);
	// clear back buffer and put this new colour in it
	glClear(GL_COLOR_BUFFER_BIT);
	// swap back and front buffers
	glfwSwapBuffers(window);

	//--------------------------------------------------------------------------------------
	//---------------------------------------MAIN LOOP--------------------------------------
	//--------------------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window)) {

		glClearColor(0.35f, 0.17f, 0.65f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL to use this shader program
		shaderProgram.Activate();
		// Bind the created VAO so OpenGL will use it
		VAO1.Bind();
		// Draw triangle using GL_TRIANGLES primitive
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);

		// tell glfw to process all pooled events
		glfwPollEvents();

	}



	//--------------------------------------------------------------------------------------
	//---------------------------------------CLEANUP----------------------------------------
	//--------------------------------------------------------------------------------------

	// delete all array and buffer objects
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();

	//delete shader program
	shaderProgram.Delete();

	// destroy window
	glfwDestroyWindow(window);
	// end glfw
	glfwTerminate();
	return 0;
}