#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad/glad.h>
#include "VBO.h"

class VAO {

public:
	GLuint ID;
	// Constructor creates vertex array object at ID
	VAO();

	// Link VAO to a given VBO
	void LinkAttributes(VBO& VBO, GLuint layout, GLuint components, GLuint type, GLsizeiptr stride, void* offset);
	void Bind();
	void Unbind();
	void Delete();


};

#endif

