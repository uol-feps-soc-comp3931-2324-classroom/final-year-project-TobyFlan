#include "objLoader.h"

// Return mesh object to be loaded in main.
Mesh loadOBJ(const char* path) {

	// Temp vars
	std::vector<GLuint> indices;
	std::vector <glm::vec3> vertices, normals;
	std::vector <glm::vec2> tex;

	// Temporary vertex object to return errors
	std::vector<Vertex> error_vert;
	std::vector<Texture> error_tex;


	// Attempt to open file
	FILE* file = fopen(path, "r");
	if (file == NULL) {
		// TODO:  Return an empty mesh object upon open faliure to prevent errors
		printf("File could not be found!");
		return Mesh(error_vert, indices, error_tex);
	}

	// Read file to end
	while (true){
		char lineHeader[256];

		// Read first word of line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF) {
			break;
		}

		// Parse starting line
		// If line represents a vertex
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			vertices.push_back(vertex);
		}
		// Else if line represents a texture
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 texture;
			fscanf(file, "%f %f\n", &texture.x, &texture.y);
			tex.push_back(texture);
		}
		// Else if represents a normal
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			normals.push_back(normal);
		}
		// Part representing indicies.
		// Is represented as f v/t/n v/t/n v/t/n
		// for vertex/texture/normal respectively
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vert1, vert2, vert3;
			unsigned int vertIndex[3], texIndex[3], normIndex[3];
			// OBJ file has all 3 indexed. My code only needs vertex
			// Index but will take them all anyway for possible use later
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertIndex[0], &texIndex[0], &normIndex[0], &vertIndex[1], &texIndex[1], &normIndex[1], &vertIndex[2], &texIndex[2], &normIndex[2]);
			// Sanity check on successful read
			if (matches != 9) {
				printf("Error: File faces cannot be read \n");
				return Mesh(error_vert, indices, error_tex);
			}
			indices.push_back((GLuint)vertIndex[0]);
			indices.push_back((GLuint)vertIndex[1]);
			indices.push_back((GLuint)vertIndex[2]);
		}


	}

	// TODO; something is darstadly wrong with the indices



	// Now we use data to load it into a Mesh object
	// Create Vertex array with all data
	std::vector<Vertex> vertexObjs = assembleVertices(vertices, normals, tex);
	
	// Create empty Texture array as we have no textures to apply
	std::vector<Texture> textures;

	// Combine vertices, indices and textures inot a mesh object
	Mesh mesh = Mesh(vertexObjs, indices, textures);

	// Return mesh object for drawing
	return mesh;
}


// Function to transform vectors into Vertex vector object.
std::vector<Vertex> assembleVertices
(
	std::vector<glm::vec3> positions,
	std::vector<glm::vec3> normals,
	std::vector<glm::vec2> tex
) {

	std::vector<Vertex> vertices;

	for (int i = 0; i < positions.size(); i++) {
		vertices.push_back(
			Vertex{
				positions[i],
				normals[i],
				glm::vec3(1.f,1.f,1.f), // Default colour to white
				tex[i]
			}
		);
	}

	return vertices;

}