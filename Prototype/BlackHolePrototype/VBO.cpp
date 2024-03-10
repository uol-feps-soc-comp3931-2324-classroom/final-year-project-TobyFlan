#include "VBO.h"

VBO::VBO(std::vector <Vertex>& vertices) {

	// Generate buffers for OpenGL to use
	glGenBuffers(1, &ID);
	// Bind buffers so OpenGL knows to use them
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	// Set data inside of VBO object
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

}

void VBO::Bind() {

	glBindBuffer(GL_ARRAY_BUFFER, ID);

}

void VBO::Unbind() {

	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void VBO::Delete() {

	glDeleteBuffers(1, &ID);

}