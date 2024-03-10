#include "Mesh.h"

Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures) {

	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;

	// Generate VAO object and bind it
	VAO.Bind();

	// Generate VBO and link to given vertices
	VBO VBO(vertices);
	// Generate EBO and link to given indices
	EBO EBO(indices);

	// Link VAO1 to VBO1
	// COORDINATES
	VAO.LinkAttributes(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	// NORMALS
	VAO.LinkAttributes(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	// COLOURS
	VAO.LinkAttributes(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	// TEXTURES
	VAO.LinkAttributes(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));
	// Unbind all Objs
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();

}


void Mesh::Draw(Shader& shader, Camera& camera) {

	shader.Activate();
	VAO.Bind();

	unsigned int numDiffuse = 0, numSpecular = 0;

	for (unsigned int i = 0; i < textures.size(); i++) {
		std::string num;
		std::string type = textures[i].type;
		if (type == "diffuse") {
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular") {
			num = std::to_string(numSpecular++);
		}

		textures[i].textureUnit(shader, (type + num).c_str(), i);
		textures[i].Bind();

	}

	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	camera.Matrix(shader, "camMatrix");

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

}