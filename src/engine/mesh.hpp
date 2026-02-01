#pragma once

class Mesh {
public:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int vertexCount;
    unsigned int indexCount;

    Mesh(float* vertices, unsigned int vertexCount_p, unsigned int floatsPerVertex,
        unsigned int* indices, unsigned int indexCount_p);

    void Draw();
};