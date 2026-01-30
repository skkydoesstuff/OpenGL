#pragma once

typedef struct mesh {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int vertexCount;
    unsigned int indexCount;
} mesh;

void mesh_init(mesh* m, const float *vertices, unsigned int vertexCount, const unsigned int* indices, unsigned int indexCount);

void mesh_destroy(mesh* mesh);