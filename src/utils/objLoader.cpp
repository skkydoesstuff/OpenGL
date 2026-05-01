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
    std::string inputfile = assetDir + "objects\\" + name + ".obj";
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
        if (it != loadedMaterials.end())
            finalMaterials[m.name] = it->second;
    }

    if (!err.empty()) std::cout << err << std::endl;
    if (!ret) std::cout << "failed to load obj" << std::endl;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texcoords;
    std::vector<glm::vec3> tangents;   // add this
    std::vector<unsigned int> out_indices;
    std::vector<SubMesh> submeshes;
    std::unordered_map<int, SubMesh> materialToSubmesh;

    // vertexMap must be outside all loops
    std::unordered_map<std::string, unsigned int> vertexMap;

    for (const auto& shape : shapes) {
        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
            size_t matID = shape.mesh.material_ids[f];

            if (materialToSubmesh.find(matID) == materialToSubmesh.end()) {
                SubMesh sm;
                sm.indexOffset = out_indices.size();
                sm.indexCount = 0;
                sm.materialName = (matID < materials.size())
                    ? materials[matID].name : "default";
                materialToSubmesh[matID] = sm;
            }

            int fv = shape.mesh.num_face_vertices[f];

            // Gather the 3 vertices of the triangle first so we can compute the tangent
            struct FaceVert { glm::vec3 pos; glm::vec3 normal; glm::vec2 uv; };
            std::array<FaceVert, 3> faceVerts;

            for (int v = 0; v < fv; v++) {
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

                faceVerts[v].pos = {
                    attrib.vertices[3 * idx.vertex_index + 0],
                    attrib.vertices[3 * idx.vertex_index + 1],
                    attrib.vertices[3 * idx.vertex_index + 2]
                };
                faceVerts[v].normal = { 0, 0, 0 };
                if (idx.normal_index >= 0 && !attrib.normals.empty()) {
                    faceVerts[v].normal = {
                        attrib.normals[3 * idx.normal_index + 0],
                        attrib.normals[3 * idx.normal_index + 1],
                        attrib.normals[3 * idx.normal_index + 2]
                    };
                }
                faceVerts[v].uv = { 0, 0 };
                if (idx.texcoord_index >= 0 && !attrib.texcoords.empty()) {
                    faceVerts[v].uv = {
                        attrib.texcoords[2 * idx.texcoord_index + 0],
                        1.0f - attrib.texcoords[2 * idx.texcoord_index + 1]
                    };
                }
            }

            // Compute tangent for this triangle
            glm::vec3 edge1 = faceVerts[1].pos - faceVerts[0].pos;
            glm::vec3 edge2 = faceVerts[2].pos - faceVerts[0].pos;
            glm::vec2 deltaUV1 = faceVerts[1].uv - faceVerts[0].uv;
            glm::vec2 deltaUV2 = faceVerts[2].uv - faceVerts[0].uv;

            float denom = deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y;
            glm::vec3 tangent = { 0, 0, 0 };
            if (std::abs(denom) > 1e-6f) {
                float f2 = 1.0f / denom;
                tangent = glm::normalize(glm::vec3(
                    f2 * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x),
                    f2 * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y),
                    f2 * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z)
                ));
            }

            // Now add vertices with the computed tangent
            for (int v = 0; v < fv; v++) {
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

                std::string vertexKey = std::to_string(idx.vertex_index) + "," +
                                        std::to_string(idx.normal_index) + "," +
                                        std::to_string(idx.texcoord_index);

                auto it = vertexMap.find(vertexKey);
                if (it != vertexMap.end()) {
                    // Accumulate tangent for shared vertices
                    tangents[it->second] += tangent;
                    out_indices.push_back(it->second);
                } else {
                    unsigned int newIndex = positions.size();
                    positions.push_back(faceVerts[v].pos);
                    normals.push_back(faceVerts[v].normal);
                    texcoords.push_back(faceVerts[v].uv);
                    tangents.push_back(tangent);
                    out_indices.push_back(newIndex);
                    vertexMap[vertexKey] = newIndex;
                }
                materialToSubmesh[matID].indexCount += 1;
            }
            index_offset += fv;
        }
    }

    // Normalize accumulated tangents
    for (auto& t : tangents)
        if (glm::length(t) > 1e-6f)
            t = glm::normalize(t);

    for (auto& [id, sm] : materialToSubmesh) {
        auto it = finalMaterials.find(sm.materialName);
        if (it != finalMaterials.end()) {
            sm.material = it->second;
            sm.renderType = RenderType::Opaque;

            if (sm.material->hasOpacityMap && !sm.material->isTransparent) {
                // black/white cutout — opaque pass with discard
                sm.material->hasCutoutMap = true;
                sm.renderType = RenderType::Opaque;
            } else if (sm.material->isTransparent || sm.material->opacityValue < 1.0f) {
                // genuine transparency — blended pass
                sm.renderType = RenderType::Transparent;
            } else {
                sm.renderType = RenderType::Opaque;
            }
        }
        submeshes.push_back(sm);
    }

    // 11 floats per vertex now (pos + normal + uv + tangent)
    std::vector<float> out_vertices;
    out_vertices.reserve(positions.size() * 11);

    for (size_t i = 0; i < positions.size(); i++) {
        out_vertices.push_back(positions[i].x);
        out_vertices.push_back(positions[i].y);
        out_vertices.push_back(positions[i].z);

        out_vertices.push_back(normals[i].x);
        out_vertices.push_back(normals[i].y);
        out_vertices.push_back(normals[i].z);

        out_vertices.push_back(texcoords[i].x);
        out_vertices.push_back(texcoords[i].y);

        out_vertices.push_back(tangents[i].x);
        out_vertices.push_back(tangents[i].y);
        out_vertices.push_back(tangents[i].z);
    }

    MeshStructure meshStructure = { out_vertices, out_indices, submeshes, finalMaterials };
    return meshStructure;
}