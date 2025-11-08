#pragma once

#include "engine/shader.hpp"
#include "engine/vertexArrayBuffer.hpp"
#include "engine/arrayBuffer.hpp"

#include <stddef.h>

class Drawable {
private:
  Shader& shader;
  VertexArrayBuffer& VAO;
  ArrayBuffer& VBO;
  ArrayBuffer& EBO;

public:
  Drawable(Shader& shader, const float* vertices, const size_t vertexCount, const unsigned int* indices, size_t indexCount);

  void draw() const;
};