#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include<json/json.h>
#include "Mesh.h"

// Use Niels Lohmann's signle include json distribution src: https://github.com/nlohmann
using json = nlohmann::json;

// Model for loading model files
class Model {

public:

	Model(const char* file);

	void Draw(Shader& shader, Camera& camera);

private:

	const char* file;
	std::vector<unsigned char> data;
	json JSON;

	// Store vector of all loaded meshes
	std::vector<Mesh> meshes;

	// Info about mesh transformations
	std::vector<glm::vec3> translationMeshes;
	std::vector<glm::quat> rotationMeshes;
	std::vector<glm::vec3> scaleMeshes;
	std::vector<glm::mat4>matrixMeshes;

	// Variables to make sure textures are only loaded once
	std::vector<std::string> loadedTextureNames;
	std::vector<Texture> loadedTextures;

	void loadMesh(unsigned int meshIndex);

	// Function to traverse json file to find correct transformation matrix for objects
	void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.f));

	std::vector<unsigned char> getData();
	std::vector<float> getFloats(json accessor);
	std::vector<GLuint> getIndices(json accessor);
	std::vector<Texture> getTextures();

	std::vector<Vertex> assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texCoords);

	std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);
};

#endif

