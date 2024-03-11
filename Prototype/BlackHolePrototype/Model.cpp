#include "Model.h"


// Basic constructor for Model that reads in a .gltf file
Model::Model(const char* file) {

	std::string text = get_file_contents(file);
	JSON = json::parse(text);

	Model::file = file;
	data = getData();

	traverseNode(0);

}

// Draw models
void Model::Draw(Shader& shader, Camera& camera) {

	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].Mesh::Draw(shader, camera, matrixMeshes[i]);
	}

}

void Model::loadMesh(unsigned int meshIndex) {

	unsigned int positionAccessorIndex = JSON["meshes"][meshIndex]["primitives"][0]["attributes"]["POSITION"];
	unsigned int normalAccessorIndex = JSON["meshes"][meshIndex]["primitives"][0]["attributes"]["NORMAL"];
	unsigned int textureAccessorIndex = JSON["meshes"][meshIndex]["primitives"][0]["attributes"]["TEXCOORD_0"];
	unsigned int indicesAccessorIndex = JSON["meshes"][meshIndex]["primitives"][0]["indices"];

	// Find all vertex components in file
	std::vector<float> positionVec = getFloats(JSON["accessors"][positionAccessorIndex]);
	std::vector<glm::vec3> positions = groupFloatsVec3(positionVec);
	std::vector<float> normalsVec = getFloats(JSON["accessors"][normalAccessorIndex]);
	std::vector<glm::vec3> normals = groupFloatsVec3(normalsVec);
	std::vector<float> texturesVec = getFloats(JSON["accessors"][textureAccessorIndex]);
	std::vector<glm::vec2> textureCoords = groupFloatsVec2(texturesVec);

	std::vector<Vertex> vertices = assembleVertices(positions, normals, textureCoords);
	std::vector<GLuint> indices = getIndices(JSON["accessors"][indicesAccessorIndex]);
	std::vector<Texture> textures = getTextures();

	meshes.push_back(Mesh(vertices, indices, textures));


}

// Search through json file and extract model transformations if found
void Model::traverseNode(unsigned int nextNode, glm::mat4 matrix) {

	json node = JSON["nodes"][nextNode];

	// Search for model translations
	glm::vec3 translation = glm::vec3(0.f, 0.f, 0.f);
	if (node.find("translation") != node.end()) {
		float transValues[3];
		for (unsigned int i = 0; i < node["translation"].size(); i++) {
			transValues[i] = (node["translation"][i]);
		}

		translation = glm::make_vec3(transValues);
	}

	// Search for model rotations
	glm::quat rotation = glm::quat(1.f, 0.f, 0.f, 0.f);
	if (node.find("rotation") != node.end()) {
		float rotValues[4] = {
			// Must do in wacky order due to differences between glm and gltf
			node["rotation"][3],
			node["rotation"][0],
			node["rotation"][1],
			node["rotation"][2]
		};
		
		rotation = glm::make_quat(rotValues);
	}

	// Search for model scalings
	glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f);
	if (node.find("scale") != node.end()) {
		float scaleValues[3];
		for (unsigned int i = 0; i < node["scale"].size(); i++) {
			scaleValues[i] = (node["scale"][i]);
		}

		scale = glm::make_vec3(scaleValues);
	}

	// Search for whole trans. matrix
	glm::mat4 matrixNode = glm::mat4(1.f);
	if (node.find("matrix") != node.end()) {
		float matrixValues[16];
		for (unsigned int i = 0; i < node["matrix"].size(); i++) {
			matrixValues[i] = (node["matrix"][i]);
		}

		matrixNode = glm::make_mat4(matrixValues);
	}

	// Combine all transformations
	glm::mat4 trans = glm::mat4(1.f);
	glm::mat4 rot = glm::mat4(1.f);
	glm::mat4 sca = glm::mat4(1.f);

	trans = glm::translate(trans, translation);
	rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, scale);

	glm::mat4 matrixNextNode = matrix * matrixNode * trans * rot * sca;

	if (node.find("mesh") != node.end()) {

		translationMeshes.push_back(translation);
		rotationMeshes.push_back(rotation);
		scaleMeshes.push_back(scale);
		matrixMeshes.push_back(matrixNextNode);

		loadMesh(node["mesh"]);
	}

	// If gltf object has children, do same for them
	if (node.find("children") != node.end()) {
		for (unsigned int i = 0; i < node["children"].size(); i++) {
			traverseNode(node["children"][i], matrixNextNode);
		}
	}



}

std::vector<unsigned char> Model::getData() {

	std::string bytes;
	
	// Get uri that points to raw binary data
	std::string uri = JSON["buffers"][0]["uri"];

	std::string fileString = std::string(file);
	std::string fileDir = fileString.substr(0, fileString.find_last_of('/') + 1);
	bytes = get_file_contents((fileDir + uri).c_str());

	std::vector<unsigned char> data(bytes.begin(), bytes.end());

	return data;

}

std::vector<float> Model::getFloats(json accessor) {

	std::vector<float> floatVec;

	// Returns index of buffer view object in json file
	unsigned int bufferViewIndex = accessor.value("bufferView", 1);
	// Tells us how many floats we need to get from the bufferView
	unsigned int count = accessor["count"];
	// Where to find data with respect to the beginning of the bufferView
	unsigned int accessorByteOffset = accessor.value("byteOffset", 0);
	// Type of float (scalar, vec2, vec3 or vec4)
	std::string type = accessor["type"];

	json bufferView = JSON["bufferViews"][bufferViewIndex];
	// Tells us where the data starts in the bufferView
	unsigned int byteOffset = bufferView.value("byteOffset", 0);

	// Use our found type to help iterate through data
	unsigned int floatsPerVertex;
	if (type == "SCALAR") floatsPerVertex = 1;
	else if (type == "VEC2") floatsPerVertex = 2;
	else if (type == "VEC3") floatsPerVertex = 3;
	else if (type == "VEC4") floatsPerVertex = 4;
	else throw std::invalid_argument("Error in reading .gltf file; Type not valid");

	unsigned int dataStart = byteOffset + accessorByteOffset;
	unsigned int dataLength = count * 4 * floatsPerVertex;

	for (unsigned int i = dataStart; i < dataStart + dataLength; i) {

		unsigned char bytes[] = { data[i++], data[i++], data[i++] , data[i++] };
		float value;
		std::memcpy(&value, bytes, sizeof(float));
		floatVec.push_back(value);

	}

	return floatVec;

}

std::vector<GLuint> Model::getIndices(json accessor) {


	std::vector<GLuint> indices;

	// Returns index of buffer view object in json file
	unsigned int bufferViewIndex = accessor.value("bufferView", 1);
	// Tells us how many floats we need to get from the bufferView
	unsigned int count = accessor["count"];
	// Where to find data with respect to the beginning of the bufferView
	unsigned int accessorByteOffset = accessor.value("byteOffset", 0);
	unsigned int componentType = accessor["componentType"];

	json bufferView = JSON["bufferViews"][bufferViewIndex];
	// Tells us where the data starts in the bufferView
	unsigned int byteOffset = bufferView.value("byteOffset", 0);

	unsigned int dataStart = byteOffset + accessorByteOffset;

	if (componentType == 5125) {

		for (unsigned int i = dataStart; i < dataStart + count * 4; i) {

			unsigned char bytes[] = { data[i++], data[i++], data[i++] , data[i++] };
			unsigned int value;
			std::memcpy(&value, bytes, sizeof(unsigned int));
			indices.push_back((GLuint)value);

		}

	}
	else if (componentType == 5123) {

		for (unsigned int i = dataStart; i < dataStart + count * 2; i) {

			unsigned char bytes[] = { data[i++], data[i++], data[i++] , data[i++] };
			unsigned short value;
			std::memcpy(&value, bytes, sizeof(unsigned short));
			indices.push_back((GLuint)value);

		}
	}
	else if (componentType == 5122) {

		for (unsigned int i = dataStart; i < dataStart + count * 2; i) {

			unsigned char bytes[] = { data[i++], data[i++], data[i++] , data[i++] };
			short value;
			std::memcpy(&value, bytes, sizeof(short));
			indices.push_back((GLuint)value);

		}
	}

	return indices;

}

std::vector<Texture> Model::getTextures() {

	std::vector<Texture> textures;

	std::string fileString = std::string(file);
	std::string fileDir = fileString.substr(0, fileString.find_last_of('/') + 1);

	for (unsigned int i = 0; i < JSON["images"].size(); i++) {

		std::string texturePath = JSON["images"][i]["uri"];

		// Check if texture is already loaded
		bool loaded = false;
		for (unsigned int j = 0; j < loadedTextureNames.size(); j++) {

			if (loadedTextureNames[j] == texturePath) {
				textures.push_back(loadedTextures[j]);
				loaded = true;
				break;
			}

		}

		if (loaded) return textures;
		// Identify if texture is diffuse or spec map
		if (texturePath.find("baseColor") != std::string::npos) { // Is a diffuse texture
			Texture diffuse = Texture((fileDir + texturePath).c_str(), "diffuse", loadedTextures.size());
			textures.push_back(diffuse);
			
		}
		else if (texturePath.find("metallicRoughness") != std::string::npos) { // Is a spec map
			Texture specular = Texture((fileDir + texturePath).c_str(), "specular", loadedTextures.size());
			textures.push_back(specular);
			loadedTextures.push_back(specular);	
		}
		loadedTextureNames.push_back(texturePath);
	}

	return textures;

}


std::vector<Vertex> Model::assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texCoords) {

	std::vector<Vertex> vertices;
	for (int i = 0; i < positions.size(); i++) {

		vertices.push_back(Vertex{ positions[i], normals[i], glm::vec3(1.f,1.f,1.f), texCoords[i] });

	}

	return vertices;

}


std::vector<glm::vec2> Model::groupFloatsVec2(std::vector<float> floatVec) {

	std::vector<glm::vec2> vectors;
	for (int i = 0; i < floatVec.size(); i) {
		vectors.push_back(glm::vec2(floatVec[i++], floatVec[i++]));
	}
	return vectors;

}
std::vector<glm::vec3> Model::groupFloatsVec3(std::vector<float> floatVec) {

	std::vector<glm::vec3> vectors;
	for (int i = 0; i < floatVec.size(); i) {
		vectors.push_back(glm::vec3(floatVec[i++], floatVec[i++], floatVec[i++]));
	}
	return vectors;

}
std::vector<glm::vec4> Model::groupFloatsVec4(std::vector<float> floatVec) {

	std::vector<glm::vec4> vectors;
	for (int i = 0; i < floatVec.size(); i) {
		vectors.push_back(glm::vec4(floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++]));
	}
	return vectors;

}
