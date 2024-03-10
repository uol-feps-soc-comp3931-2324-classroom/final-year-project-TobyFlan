#include "Camera.h"



// Basic constructor for camera obj
Camera::Camera(int width, int height, glm::vec3 position) {

	Camera::width = width;
	Camera::height = height;
	Position = position;

}

// Matrix function that sends info to shader program
void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane) {

	// Init vectors 
	glm::mat4 view = glm::mat4(1.f);
	glm::mat4 proj = glm::mat4(1.f);

	// Forces camera to look in roght direction from right position
	view = glm::lookAt(Position, Position + Orientation, Up);
	// Calculate perspective 
	proj = glm::perspective(glm::radians(FOVdeg), (float)(width / height), nearPlane, farPlane);

	// Pack all info up into a variable
	cameraMatrix = proj * view;
	

}

void Camera::Matrix(Shader& shader, const char* uniform) {

	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));

}

void Camera::Inputs(GLFWwindow* window) {

	// Controls for moving left/right/up/down
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		Position += speed * Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		Position -= speed * glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		Position -= speed * Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		Position += speed * glm::normalize(glm::cross(Orientation, Up));
	}

	// Controls for moving up/down
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		Position += speed * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		Position -= speed * Up;
	}

	// Controls for making camera move faster
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		speed = 0.5f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
		speed = 0.1f;
	}

	// Mouse controls for changing direction of camera

	// Only move mouse when left ckick is being held
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Set cursor pos to center on furst click to stop mouse from jumping
		if (firstClick) {
			glfwSetCursorPos(window, (float)width / 2, (float)height / 2);
			firstClick = false;
		}

		// Store mouse x and y positions
		double mouseX, mouseY;

		// Get current mouse position and store
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Get rotation based off of X and Y mouse coords

		// Rotation around the x axis (looking up and down)
		float rotationX = sensitivity * (float)(mouseY - (height / 2)) / height;
		// Rotation around the y axis (side to size)
		float rotationY = sensitivity * (float)(mouseX - (height / 2)) / height;

		// Store new mouse orientation for error checks (make sure it cant do a 360 deg flip)
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotationX), glm::normalize(glm::cross(Orientation, Up)));

		// Lock rotation around X axis
		if (!((glm::angle(newOrientation, Up) <= glm::radians(5.f)) || (glm::angle(newOrientation, -Up) <= glm::radians(5.f)))) {

			Orientation = newOrientation;

		}

		// Always allow rotation around Y axis
		Orientation = glm::rotate(Orientation, glm::radians(-rotationY), Up);

		// Center mouse position on window
		glfwSetCursorPos(window, (float)width / 2, (float)height / 2);


	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Reset first click to true to prevent next jumps
		firstClick = true;

	}

	// Pressing esc exits the program
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

}
