#include "engine/drawable.hpp"
#include "engine/shader.hpp"
#include "engine/vertexArrayBuffer.hpp"
#include "engine/arrayBuffer.hpp"

#include <glad/glad.h>

Drawable::Drawable(
  Shader& shader,
  const float* vertices,
  const size_t vertexCount,
  const unsigned int* indices,
  const size_t indexCount
)
  : shader(shader),
    VAO(*(new VertexArrayBuffer(shader))),
    VBO(*(new ArrayBuffer(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertices))),
    EBO(*(new ArrayBuffer(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices)))
{
  VAO.bind();
  VBO.bind();
  EBO.bind();
  VAO.setupVertexAttribute(0, 3, 3 * sizeof(float), 0);
  VBO.unbind();
  VAO.unbind();
}

void Drawable::draw() const {
  shader.bind();
  VAO.bind();
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
  VAO.unbind();
}
