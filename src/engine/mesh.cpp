#include <GLAD/glad.h>

#include <mesh.hpp>

Mesh::Mesh(float* vertices, unsigned int vertexCount_p, unsigned int floatsPerVertex,
    unsigned int* indices, unsigned int indexCount_p)
{
    this->vertexCount = vertexCount_p;
    this->indexCount  = indexCount_p;

    unsigned int strideBytes = floatsPerVertex * sizeof(float);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        this->vertexCount * floatsPerVertex * sizeof(float),
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

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        strideBytes,
        (void*)0
    );

    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE,
        strideBytes,
        (void*)(3 * sizeof(float))
    );

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Mesh::Draw() {
    glBindVertexArray(this->VAO);
    
    if (this->indexCount > 0) {
        glDrawElements(GL_TRIANGLES, this->indexCount, GL_UNSIGNED_INT, (void*)0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, this->vertexCount);
    }
}