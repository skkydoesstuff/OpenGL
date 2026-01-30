#include <glad/glad.h>

#include "mesh.h"

void mesh_init(mesh* m,
               const float* vertices,
               unsigned int vertexCount,
               const unsigned int* indices,
               unsigned int indexCount)
{
    m->vertexCount = vertexCount;
    m->indexCount  = indexCount;

    glGenVertexArrays(1, &m->VAO);
    glGenBuffers(1, &m->VBO);

    glBindVertexArray(m->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m->VBO);

    glBufferData(GL_ARRAY_BUFFER,
                 vertexCount * sizeof(float),
                 vertices,
                 GL_STATIC_DRAW);

    if (indices && indexCount > 0) {
        glGenBuffers(1, &m->EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     indexCount * sizeof(unsigned int),
                     indices,
                     GL_STATIC_DRAW);
    } else {
        m->EBO = 0;
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
}

void mesh_destroy(mesh* m)
{
    if (m->EBO) glDeleteBuffers(1, &m->EBO);
    if (m->VBO) glDeleteBuffers(1, &m->VBO);
    if (m->VAO) glDeleteVertexArrays(1, &m->VAO);
}