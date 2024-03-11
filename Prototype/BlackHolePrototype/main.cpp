
// base surrounding code for opengl project based off of
// tutorial by Victor Gordan on youtube "OpenGL Tutorial"

#include "Model.h"


// Global constants for window width and height
const unsigned int width = 800;
const unsigned int height = 800;


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
		glfwTerminate();
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


	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);


	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


	// Make 3d triangels draw in correct order upon rotation
	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.f, 0.f, 2.f));


	// FPS counter
	double prevTime = 0.0;
	double currentTime = 0.0;
	double timeDiff;
	unsigned int counter = 0;


	// Load example model
	Model model("models/map/scene.gltf");



	//--------------------------------------------------------------------------------------
	//---------------------------------------MAIN LOOP--------------------------------------
	//--------------------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window)) {

		currentTime = glfwGetTime();
		// Calculate time per frame
		timeDiff = currentTime - prevTime;
		counter++;
		if (timeDiff >= 1.0 / 30.0) {
			// Find frames per second
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);

			// Find time per frame
			std::string msPerFrame = std::to_string((timeDiff / counter) * 1000);

			// Display FPS counter in window title
			std::string title = ("Blackhole Ray Tracer // " + FPS + " FPS  /" + msPerFrame + " ms per frame");
			glfwSetWindowTitle(window, title.c_str());
			prevTime = currentTime;
			counter = 0;
		}

		glClearColor(0.35f, 0.17f, 0.65f, 1.f);
		// Clear colour buffer and depth buffer to be rewritten next frame swap
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Recieve inputs for camera
		camera.Inputs(window);

		// Calculate view matrices and send them to shader file
		camera.updateMatrix(45.f, 0.1f, 100.f);

		model.Draw(shaderProgram, camera);

		// Swap buffers to change scene
		glfwSwapBuffers(window);
		// Tell glfw to process all pooled events
		glfwPollEvents();

	}



	//--------------------------------------------------------------------------------------
	//---------------------------------------CLEANUP----------------------------------------
	//--------------------------------------------------------------------------------------



	// Delete shader program
	shaderProgram.Delete();

	// Destroy window
	glfwDestroyWindow(window);
	// End glfw
	glfwTerminate();
	return 0;
}