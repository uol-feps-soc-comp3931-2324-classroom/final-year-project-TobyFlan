#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glm/glm.hpp>
#include<glad/glad.h>
#include<vector>


struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec3 colour;
	glm::vec2 textureUV;
};


class VBO {

	public:
		GLuint ID;
		// Constructor generates VBO object for a given set of vertices
		VBO(std::vector <Vertex>& vertices);

		void Bind();
		void Unbind();
		void Delete();


};

#endif
