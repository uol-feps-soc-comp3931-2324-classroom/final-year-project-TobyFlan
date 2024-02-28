
// base surrounding code for opengl project based off of
// tutorial by Victor Gordan on youtube "OpenGL Tutorial"

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>

#include "Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"


// Manually make vertices for rendering a square
GLfloat vertices[] = {
	//               POSITIONS               //           COLOURS           //		 TEXTURE MAP		 //
	            -0.5f, -0.5f, 0.f,                   0.8f,   0.8f,  0.2f,            1.f, 0.f,    // bottom right
	            0.5f,  -0.5f, 0.f,                   0.55f,  0.2f,  0.02f,           1.f, 1.f,	  // top right
	            0.5f,   0.5f, 0.f,                   0.321f, 0.01f, 0.4f,            0.f, 1.f,    // top left
		        -0.5f,  0.5f, 0.f,                   0.11f, 0.11f, 0.14f,            0.f, 0.f,    // bottom left
 
};

// Set index buffer to write order that OpenGL should write vertices in
GLuint indices[] = {
	0, 2, 1, // upper triangle of square
	0, 3, 2  // lower
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
	GLFWwindow* window = glfwCreateWindow(800, 800, "BlackHolePrototype", NULL, NULL);
	if (window == NULL) {

		std::cout << "Error: Failed to create GLFW window" << std::endl;
		return -1;

	}
	// Now we have a window, we must tell glfw to use it (very silly)
	glfwMakeContextCurrent(window);


	// Load GLAD
	gladLoadGL();

	// Specify viewport of OpenGL in the window
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

		// Send uniforms to .vert shader
		glUniform1f(uniformID, 0.f);
		// Bind texture object
		tiles.Bind();

		// Bind the created VAO so OpenGL will use it
		VAO1.Bind();
		// Draw triangle using GL_TRIANGLES primitive
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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