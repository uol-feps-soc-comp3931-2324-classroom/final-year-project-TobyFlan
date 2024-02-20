#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glad/glad.h>

class VBO {

	public:
		GLuint ID;
		// Constructor generates VBO object for a given set of vertices
		VBO(GLfloat* vertices, GLsizeiptr size);

		void Bind();
		void Unbind();
		void Delete();


};

#endif
