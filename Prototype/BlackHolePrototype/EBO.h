#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include<glad/glad.h>
#include<vector>

class EBO {

public:
	GLuint ID;
	// Constructor creates an Element Buffer Object and links it to a given set of indices
	EBO(std::vector <GLuint>& indices);

	void Bind();
	void Unbind();
	void Delete();

};

#endif

