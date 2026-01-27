#pragma once

#include <stdint.h>

typedef struct mesh {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    uint32_t vertexCount;
    uint32_t indexCount;
} mesh;

void drawMesh(mesh* shape);