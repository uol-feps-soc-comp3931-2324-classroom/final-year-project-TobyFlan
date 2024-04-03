#ifndef OBJECT_LOADER_H
#define OBJECT_LOADER_H

#include "Mesh.h"

// Returns mesh object of .obj file to be drawn
Mesh loadOBJ(const char* path);

// Function to transform vectors into Vertex vector object.
std::vector<Vertex> assembleVertices
(
	std::vector<glm::vec3> positions,
	std::vector<glm::vec3> normals,
	std::vector<glm::vec2> tex
);


#endif
