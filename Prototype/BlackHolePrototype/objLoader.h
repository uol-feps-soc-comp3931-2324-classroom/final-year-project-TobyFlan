#ifndef OBJECT_LOADER_H
#define OBJECT_LOADER_H

#include "Mesh.h"

// A hash function for vertices to use in an unordered map
struct VertexHash {
    size_t operator()(const Vertex& v) const {
        return std::hash<float>()(v.pos.x) ^ std::hash<float>()(v.pos.y) ^ std::hash<float>()(v.pos.z) ^
            std::hash<float>()(v.textureUV.x) ^ std::hash<float>()(v.textureUV.y) ^
            std::hash<float>()(v.normal.x) ^ std::hash<float>()(v.normal.y) ^ std::hash<float>()(v.normal.z) ^
            std::hash<float>()(v.colour.x) ^ std::hash<float>()(v.colour.y) ^ std::hash<float>()(v.colour.z);
    }
};

// Override == to check if vertex exists
inline bool operator==(const Vertex& a, const Vertex& b) {
    return a.pos == b.pos && a.normal == b.normal && a.textureUV == b.textureUV && a.colour == b.colour;
}

// Returns mesh object of .obj file to be drawn
Mesh loadOBJ(const char* path, float scale, std::vector <Texture> tex);


#endif
