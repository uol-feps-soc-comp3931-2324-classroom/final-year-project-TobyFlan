#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

// base surrounding code for opengl project based off of
// tutorial by Victor Gordan on youtube "OpenGL Tutorial"


//initial simple shaders
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0 ) in vec3 aPos;\n"
"void main(){\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColour;\n"
"void main(){\n"
"	FragColour = vec4(0.9f, 0.1f, 0.1f, 1.f);\n"
"}\n\0";



int main() {

	// init glfw
	glfwInit();

	// tell glfw about itself so it knows what version we are using
	// we are using OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// We are using the CORE profile, which is not backwards compatible
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//manually make vertices for rendering a triangle
	GLfloat vertices[] = {
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.f, //no z because 2d atm
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.f,
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.f
	};

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


	// create reference to shaders

	// create vertex shader object and get refrence
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// attach vertex shader to vertex shader object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// compile vertex shader
	glCompileShader(vertexShader);

	// create frag shader object and get refrence
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// attach frag shader to frag shader object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// compile frag shader
	glCompileShader(fragmentShader);

	// wrap these up into a shader program object
	GLuint shaderProgram = glCreateProgram();
	// Attach Vertex and Frag shaders together into the Shader Program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Link all shaders together into Shader Program
	glLinkProgram(shaderProgram);

	// delete old shaders now
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//--------------------------------------------------------------------------------------
	//---------------------------------------VBOS and stuff---------------------------------
	//--------------------------------------------------------------------------------------

	// create reference for Vertex Array Obj and Vertex Buffer Obj
	GLuint VAO, VBO;

	// Generate VAO and VBO with 1 object each
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Make VAO the current VAO object by binding it
	glBindVertexArray(VAO);

	// Bind VBO specifying its an ARRAY BUFFER.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Introduce the written verticies into the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Configure Vertex Attribute so that OpenGL can read the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// Enable the vertex attribute so OpenGL knows it should use it
	glEnableVertexAttribArray(0);

	// Bind VBO and VAO so that we dont acidentially modify them
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


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
		glUseProgram(shaderProgram);
		// Bind the created VAO so OpenGL will use it
		glBindVertexArray(VAO);
		// Draw triangle using GL_TRIANGLES primitive
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);

		// tell glfw to process all pooled events
		glfwPollEvents();

	}



	//--------------------------------------------------------------------------------------
	//---------------------------------------CLEANUP----------------------------------------
	//--------------------------------------------------------------------------------------
	// destroy vao / vbo objects
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// destroy window
	glfwDestroyWindow(window);
	// end glfw
	glfwTerminate();
	return 0;
}