#include <GLAD/glad.h>
#include "mesh.h"

typedef struct mesh {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    uint32_t vertexCount;
    uint32_t indexCount;
} mesh;

void drawMesh(mesh* shape) {
    glBindVertexArray(shape->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, shape->VBO);
    if (!shape->EBO) {
        glDrawArrays(GL_TRIANGLES, 0, shape->vertexCount);
    } else {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shape->EBO);
        glDrawElements(GL_TRIANGLES, shape->indexCount, GL_UNSIGNED_INT, 0);
        
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}