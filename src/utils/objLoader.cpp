#include "utils/objLoader.hpp"
#include "utils/fileUtils.hpp"
#include "utils/mtlLoader.hpp"
#include "utils/tiny_obj_loader.h"

#include "core/renderer/mesh.hpp"

#include <glm/glm.hpp>

#include <iostream>

MeshStructure loadOBJ(const std::string& name) {
    static std::string exeDir = getExecutableDirectory();
    static std::string assetDir = exeDir + "\\assets\\";
    std::string inputfile = assetDir + "\\objects\\" + name + ".obj";
    static std::string mtlDir = assetDir + "\\objects\\";
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str(), mtlDir.c_str());

    auto mtlPath = assetDir + "objects\\" + name + ".mtl";

    auto loadedMaterials = MTLLoader::load(mtlPath);

    std::unordered_map<std::string, Material*> finalMaterials;

    for (auto& m : materials) {
        auto it = loadedMaterials.find(m.name);
        if (it != loadedMaterials.end()) {
            finalMaterials[m.name] = it->second;
        }
    }

    if (!err.empty()) {
        std::cout << err << std::endl;
    }

    if (!ret) {
        std::cout << "failed to load obj" << std::endl;
    }

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texcoords;
    std::vector<unsigned int> out_indices;
    
    std::vector<SubMesh> submeshes;

    std::unordered_map<int, SubMesh> materialToSubmesh;

    for (const auto& shape : shapes) {
        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
            size_t matID = shape.mesh.material_ids[f];

            if (materialToSubmesh.find(matID) == materialToSubmesh.end()) {
                SubMesh sm;
                sm.indexOffset = out_indices.size();
                sm.indexCount = 0;
                sm.materialName = (matID < materials.size())
                    ? materials[matID].name
                    : "default";

                materialToSubmesh[matID] = sm;
            }

            int fv = shape.mesh.num_face_vertices[f];  // Should be 3 for triangles
            
            // Process each vertex in the face
            for (int v = 0; v < fv; v++) {
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                
                // Get position (always exists)
                glm::vec3 pos = {
                    attrib.vertices[3 * idx.vertex_index + 0],
                    attrib.vertices[3 * idx.vertex_index + 1],
                    attrib.vertices[3 * idx.vertex_index + 2]
                };
                
                // Get normal (if available)
                glm::vec3 normal = {0.0f, 0.0f, 0.0f};
                if (idx.normal_index >= 0 && !attrib.normals.empty()) {
                    normal = {
                        attrib.normals[3 * idx.normal_index + 0],
                        attrib.normals[3 * idx.normal_index + 1],
                        attrib.normals[3 * idx.normal_index + 2]
                    };
                }
                
                // Get texcoord (if available, with Vulkan Y-flip)
                glm::vec2 texcoord = {0.0f, 0.0f};
                if (idx.texcoord_index >= 0 && !attrib.texcoords.empty()) {
                    texcoord = {
                        attrib.texcoords[2 * idx.texcoord_index + 0],
                        1.0f - attrib.texcoords[2 * idx.texcoord_index + 1]  // Flip Y for Vulkan/OpenGL
                    };
                }
                
                // Build a key to find if this vertex already exists
                // (using position + normal + texcoord for exact matching)
                std::string vertexKey = std::to_string(idx.vertex_index) + "," +
                                        std::to_string(idx.normal_index) + "," +
                                        std::to_string(idx.texcoord_index);
                
                // Simple deduplication map (use unordered_map for better performance)
                std::unordered_map<std::string, unsigned int> vertexMap;
                
                auto it = vertexMap.find(vertexKey);
                if (it != vertexMap.end()) {
                    // Vertex already exists - use existing index
                    out_indices.push_back(it->second);
                } else {
                    // New vertex - add to buffers
                    unsigned int newIndex = positions.size();
                    
                    positions.push_back(pos);
                    normals.push_back(normal);
                    texcoords.push_back(texcoord);
                    
                    out_indices.push_back(newIndex);
                    materialToSubmesh[matID].indexCount += 1;
                    vertexMap[vertexKey] = newIndex;
                }
            }
            index_offset += fv;
        }
    }

    for (auto& [id, sm] : materialToSubmesh) {
        auto it = finalMaterials.find(sm.materialName);

        if (it != finalMaterials.end()) {
            sm.material = it->second;
            sm.renderType =
                (sm.material->opacity < 1.0f)
                ? RenderType::Transparent
                : RenderType::Opaque;
        }

        submeshes.push_back(sm);
    }

    std::vector<float> out_vertices = {};
    out_vertices.reserve(positions.size() * 8);  // 8 floats per vertex
    
    for (size_t i = 0; i < positions.size(); i++) {
        // Position (3 floats)
        out_vertices.push_back(positions[i].x);
        out_vertices.push_back(positions[i].y);
        out_vertices.push_back(positions[i].z);
        
        // Normal (3 floats)
        out_vertices.push_back(normals[i].x);
        out_vertices.push_back(normals[i].y);
        out_vertices.push_back(normals[i].z);
        
        // Texcoord (2 floats)
        out_vertices.push_back(texcoords[i].x);
        out_vertices.push_back(texcoords[i].y);
    }

    MeshStructure meshStructure = {out_vertices, out_indices, submeshes, finalMaterials};

    return meshStructure;
}