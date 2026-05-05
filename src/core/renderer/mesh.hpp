#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "core/renderer/material.hpp"

enum class RenderType {
    Opaque,
    Transparent
};

struct SubMesh {
    uint32_t indexOffset = 0;
    uint32_t indexCount = 0;
    std::shared_ptr<Material> material = nullptr;
    std::string materialName = "";

    glm::vec3 boundsMin;
    glm::vec3 boundsMax;
    glm::vec3 boundsCenter;

    RenderType renderType;
};

class Mesh {
public:
    Mesh(std::vector<float> vertices, uint32_t stride, std::vector<unsigned int> indices = {});
    ~Mesh();

    void computeBounds(SubMesh& sm);

    void addVertexAttribute(uint32_t index, uint32_t attribSize, unsigned int attribType, GLsizei stride, const void* offset);

    void draw() const;
    void drawSubMesh(const SubMesh& sm) const;

    void setMaterial(std::shared_ptr<Material> mat, const std::string& name = "default");

    std::vector<float> getVertices() { return this->vs; }
    std::vector<unsigned int> getIndices() { return this->is; }
    uint32_t getStride() { return this->stride; }

    std::vector<SubMesh> submeshes; // <-- add this
    std::string sourceFile;

    Mesh(const Mesh &) = delete;
    Mesh& operator=(const Mesh &) = delete;

private:
    unsigned int VAO, VBO, EBO;
    std::vector<float> vs;
    std::vector<unsigned int> is;
    uint32_t stride;

    void bindVAO() const;
};
