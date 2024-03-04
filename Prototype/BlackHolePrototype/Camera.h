#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H


#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include "shaderClass.h"

class Camera {

public:
	glm::vec3 Position;
	// Define starting camera orientation (direction it is facing)
	glm::vec3 Orientation = glm::vec3(0.f, 0.f, -1.f);
	// Define "up" direction of camera
	glm::vec3 Up = glm::vec3(0.f, 1.f, 0.f);

	// Bool to help prevent camera jumping when switching on/off rotation
	bool firstClick = true;

	int width;
	int height;

	float speed = 0.1f;
	float sensitivity = 100.f;

	// Simple constructor
	Camera(int width, int height, glm::vec3 position);

	// Matrix function to send view and projection matrices to shader
	void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform);

	// Input handler for camera
	void Inputs(GLFWwindow* window);

};


#endif
