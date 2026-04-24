#pragma once

#include <glad/glad.h>
#include <vector>

class Mesh {
public:
    Mesh(std::vector<float> vertices, uint32_t stride, std::vector<unsigned int> indices = {});

    ~Mesh();
    
    void addVertexAttribute(uint32_t index, uint32_t attribSize, unsigned int attribType, GLsizei stride, const void* offset);

    void draw();

    Mesh(const Mesh &) = delete;
    Mesh& operator=(const Mesh &) = delete;

private:
    unsigned int VAO, VBO, EBO;
    std::vector<float> vs;
    std::vector<unsigned int> is;
    uint32_t stride;
    void bindVAO() const;
};