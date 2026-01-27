#include <GLAD/glad.h>
#include "mesh.h"

typedef struct mesh {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    uint32_t vertexCount;
    uint32_t indexCount;
} mesh;

mesh createMesh(float vertices[], unsigned int vertexCount, unsigned int indices[], unsigned int indexCount) {
    mesh shape = {0};
    glGenVertexArrays(1, &shape.VAO);
    glBindVertexArray(shape.VAO);
    
    glGenBuffers(1, &shape.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, shape.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertices, GL_STATIC_DRAW);

    
    if (indices != NULL) {
        glGenBuffers(1, &shape.EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shape.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);
        shape.indexCount = indexCount;
    }
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    shape.vertexCount = vertexCount / 3;

    glBindVertexArray(0);
    return shape;
}

void drawMesh(mesh* shape) {
    glBindVertexArray(shape->VAO);

    if (shape->EBO)
        glDrawElements(GL_TRIANGLES, shape->indexCount, GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, shape->vertexCount);

    glBindVertexArray(0);
}