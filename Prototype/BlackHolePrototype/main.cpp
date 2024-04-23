
// base surrounding code for opengl project based off of
// tutorial by Victor Gordan on youtube "OpenGL Tutorial"

#include"objLoader.h"

// TEMP CODE TO LIMIT FRAMERATE CUZ MY LAPTOP IS TOO FAST
#include <Windows.h>


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

	Texture textures[]{

		// Texture stuff
		Texture("empty_workshop.jpg", "diffuse", 1, GL_RGB, GL_UNSIGNED_BYTE)
		//Texture("HDR_blue_nebulae-1.hdr", "diffuse", 1, GL_RGB, GL_UNSIGNED_BYTE)
		//Texture("HDR_hazy_nebulae.hdr", "diffuse", 1, GL_RGB, GL_UNSIGNED_BYTE)
		//Texture("HDR_multi_nebulae.hdr", "diffuse", 1, GL_RGB, GL_UNSIGNED_BYTE)
		//Texture("HDR_silver_and_gold_nebulae.hdr", "diffuse", 1, GL_RGB, GL_UNSIGNED_BYTE)
		//Texture("HDR_subdued_blue_nebulae.hdr", "diffuse", 1, GL_RGB, GL_UNSIGNED_BYTE)
		//Texture("milkyway_texture.jpg", "diffuse", 1, GL_RGB, GL_UNSIGNED_BYTE)
		//Texture("tile_floor.jpg", "diffuse", 1, GL_RGB, GL_UNSIGNED_BYTE)

	};

	// Generate shader program obj using Vert/Frag shader pipeline
	Shader shaderProgram("default.vert", "default.frag");


	//--------------------------------------------------------------------------------------
	//---------------------------------------VBOS and stuff---------------------------------
	//--------------------------------------------------------------------------------------


	



	// Load sphere obj file and create mesh for it
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	Mesh sphere = loadOBJ("models/sphere.txt", 20000.f, tex);



	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);



	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));

	// Estimate sphere radius and center
	glm::vec3 sphereCenter(0.f, 0.f, 0.f);
	//float sphereRadius = 5000000.f;

	glUniform3fv(glGetUniformLocation(shaderProgram.ID, "sphereCenter"), 1, glm::value_ptr(sphereCenter));
	glUniform1f(glGetUniformLocation(shaderProgram.ID, "sphereRadius"), 10000.f);









	// Make 3d triangels draw in correct order upon rotation
	glEnable(GL_DEPTH_TEST);


	Camera camera(width, height, glm::vec3(0.f, 0.f, 300.f));


	// FPS counter
	double prevTime = 0.0;
	double currentTime = 0.0;
	double timeDiff;
	unsigned int counter = 0;


	// TEMP CODE TO LIMIT FRAMERATE CUZ MY LAPTOP IS TOO FAST
	const double targetFPS = 60.0;
	// TEMP CODE TO LIMIT FRAMERATE CUZ MY LAPTOP IS TOO FAST
	const double frameTime = 1.0 / targetFPS;

	



	//--------------------------------------------------------------------------------------
	//---------------------------------------MAIN LOOP--------------------------------------
	//--------------------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window)) {

		// TEMP CODE TO LIMIT FRAMERATE CUZ MY LAPTOP IS TOO FAST
		//double frameStartTime = glfwGetTime();

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
		camera.updateMatrix(45.f, 0.1f, 10000000.f);
		camera.getViewInverse(shaderProgram);


		// Draws different meshes
		//floor.Draw(shaderProgram, camera);
		sphere.Draw(shaderProgram, camera);
		//light.Draw(lightShader, camera);
		


		// Swap buffers to change scene
		glfwSwapBuffers(window);
		// Tell glfw to process all pooled events
		glfwPollEvents();

		// TEMP CODE TO LIMIT FRAMERATE CUZ MY LAPTOP IS TOO FAST
		/*double frameEndTime = glfwGetTime();
		double frameDuration = frameEndTime - frameStartTime;

		if (frameDuration < frameTime) {
			double sleep = frameTime - frameDuration;
			int milliseconds = static_cast<int>(sleep * 1000);
			Sleep(milliseconds);
		}*/

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