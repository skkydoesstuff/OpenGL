#include <unordered_map>
#include <vector>
#include <string>

#include "core/renderer/mesh.hpp"
#include "core/renderer/material.hpp"

struct MeshStructure {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<SubMesh> submeshes;
    std::unordered_map<std::string, Material*> materials;
};

void saveOBJ(const std::string& OBJOutFilePath, const std::string& MTLOutFilePath, std::shared_ptr<Mesh> mesh);

MeshStructure loadOBJ(const std::string& name);