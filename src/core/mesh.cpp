#include "mesh.hpp"

#include <glad/glad.h>
#include <vector>

Mesh::Mesh(std::vector<float> vertices, uint32_t stride, std::vector<unsigned int> indices): vs(vertices), is(indices), stride(stride) {
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    if (indices.size() > 0) {
        glGenBuffers(1, &this->EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    } else {
        this->EBO = 0;
    }
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void Mesh::addVertexAttribute(uint32_t index, uint32_t attribSize, unsigned int attribType, GLsizei stride, const void* offset) {
    this->bindVAO();
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO); // ← REQUIRED
    glVertexAttribPointer(index, attribSize, attribType, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(index);
}

void Mesh::draw() {
    this->bindVAO();
    if (this->is.size() > 0) {
        glDrawElements(GL_TRIANGLES, this->is.size(), GL_UNSIGNED_INT, nullptr);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, this->vs.size()/stride);
    }
}

void Mesh::bindVAO() const { glBindVertexArray(this->VAO); }
