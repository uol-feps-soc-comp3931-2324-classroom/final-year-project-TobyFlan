#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

// base surrounding code for opengl project based off of
// tutorial by Victor Gordan on youtube "OpenGL Tutorial"


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

	// Background colour of window
	glClearColor(0.07f, 0.55f, 0.17f, 1.f);
	// clear back buffer and put this new colour in it
	glClear(GL_COLOR_BUFFER_BIT);
	// swap back and front buffers
	glfwSwapBuffers(window);

	//--------------------------------------------------------------------------------------
	//---------------------------------------MAIN LOOP--------------------------------------
	//--------------------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window)) {

		// tell glfw to process all pooled events
		glfwPollEvents();

	}



	//--------------------------------------------------------------------------------------
	//---------------------------------------CLEANUP----------------------------------------
	//--------------------------------------------------------------------------------------
	// destroy window
	glfwDestroyWindow(window);
	// end glfw
	glfwTerminate();
	return 0;
}