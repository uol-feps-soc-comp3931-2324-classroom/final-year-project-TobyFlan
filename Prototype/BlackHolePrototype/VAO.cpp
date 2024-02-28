#include "VAO.h"

// Constructor creates vertex array object at ID
VAO::VAO() {

	// generate vertex arrays for OpenGL to use
	glGenVertexArrays(1, &ID);

}

// Link VAO to a given VBO
void VAO::LinkAttributes(VBO VBO, GLuint layout, GLuint components, GLuint type, GLsizeiptr stride, void* offset) {

	// Bind VBO to VAO, so OpenGL knows to use it
	VBO.Bind();
	// Configure Vertex Attribute so that OpenGL can read the VBO
	glVertexAttribPointer(layout, components, type, GL_FALSE, stride, offset);
	// Enable the vertex attribute so OpenGL knows it should use it
	glEnableVertexAttribArray(layout);
	VBO.Unbind();

}

void VAO::Bind() {

	// Make VAO the current VAO object by binding it
	glBindVertexArray(ID);

}

void VAO::Unbind() {

	glBindVertexArray(0);

}

void VAO::Delete() {

	glDeleteVertexArrays(1, &ID);

}