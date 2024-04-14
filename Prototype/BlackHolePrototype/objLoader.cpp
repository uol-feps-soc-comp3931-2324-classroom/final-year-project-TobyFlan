#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "objLoader.h" 


Mesh loadOBJ(const char* path, float scale) {
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_texCoords;
    std::vector<glm::vec3> temp_normals;

    // Default error values for returning empty mesh for error
    std::vector<Vertex> error_vertices;
    std::vector<GLuint> error_indices;
    std::vector<Texture> error_textures; 

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Cannot open file!" << std::endl;
        // Return blank mesh to avoid crashes
        return Mesh(error_vertices, error_indices, error_textures); 
    }

    std::string line;
    // Map vertex positions, normals and texUVs with indices to ensure correct ordering
    std::unordered_map<Vertex, unsigned int, VertexHash> vertexMap;
    std::vector<unsigned int> indices;
    std::vector<Vertex> vertices;

    // Read in data from obj file
    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") { // POSITIONS
            glm::vec3 vertex;
            ss >> vertex.x >> vertex.y >> vertex.z;
            temp_vertices.push_back(vertex);
        }
        else if (prefix == "vt") { // TEXCOORDS
            glm::vec2 texCoord;
            ss >> texCoord.x >> texCoord.y;
            temp_texCoords.push_back(texCoord);
        }
        else if (prefix == "vn") { // NORMALS
            glm::vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        }
        else if (prefix == "f") { // INDICES
            std::string vertex1, vertex2, vertex3;
            ss >> vertex1 >> vertex2 >> vertex3;
            int vertIndices[3], texIndices[3], normIndices[3];
            sscanf(vertex1.c_str(), "%d/%d/%d", &vertIndices[0], &texIndices[0], &normIndices[0]);
            sscanf(vertex2.c_str(), "%d/%d/%d", &vertIndices[1], &texIndices[1], &normIndices[1]);
            sscanf(vertex3.c_str(), "%d/%d/%d", &vertIndices[2], &texIndices[2], &normIndices[2]);

            // Process each vertex of the face
            for (int i = 0; i < 3; i++) {
                Vertex v{
                    temp_vertices[vertIndices[i] - 1] * scale,
                    temp_normals[normIndices[i] - 1],
                    // Default colour to white
                    glm::vec3(1.0f, 1.0f, 1.0f),
                    temp_texCoords[texIndices[i] - 1]
                };

                // Check if the vertex is in the map
                if (vertexMap.count(v) == 0) {
                    vertices.push_back(v);
                    unsigned int index = static_cast<unsigned int>(vertices.size() - 1);
                    vertexMap[v] = index;
                    indices.push_back(index);
                }
                else {
                    indices.push_back(vertexMap[v]);
                }
            }
        }
    }

    // TODO: load the textures
    std::vector<Texture> textures; 

    // Create mesh from data
    return Mesh(vertices, indices, textures);
}

