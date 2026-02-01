#include <GLAD/glad.h>

#include <mesh.hpp>

Mesh::Mesh(float* vertices, unsigned int vertexCount_p, unsigned int floatsPerVertex,
    unsigned int* indices, unsigned int indexCount_p)
{
    this->vertexCount = vertexCount_p;
    this->indexCount  = indexCount_p;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        this->vertexCount * 3 * sizeof(float),
        vertices,
        GL_STATIC_DRAW
    );

    EBO = 0;
    if (indices && this->indexCount > 0) {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            this->indexCount * sizeof(unsigned int),
            indices,
            GL_STATIC_DRAW
        );
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, floatsPerVertex * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}