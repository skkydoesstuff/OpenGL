#include <glad/glad.h>

#include "engine/vertexArrayBuffer.hpp"
#include "engine/shader.hpp"

VertexArrayBuffer::VertexArrayBuffer(Shader &shader) : m_Shader(shader) {
  glGenVertexArrays(1, &ID);
}

VertexArrayBuffer::~VertexArrayBuffer() {
  glDeleteVertexArrays(1, &ID);
}

void VertexArrayBuffer::bind() {
  glBindVertexArray(ID);
}

void VertexArrayBuffer::unbind() {
  glBindVertexArray(0);
}

void VertexArrayBuffer::draw(unsigned int bufferType, unsigned int vertexCount, unsigned int dataType) {
  m_Shader.bind();
  if (bufferType == GL_ARRAY_BUFFER) {
    bind();
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    unbind();
  } else if (bufferType == GL_ELEMENT_ARRAY_BUFFER) {
    bind();
    glDrawElements(GL_TRIANGLES, vertexCount, dataType, nullptr);
  }
  m_Shader.bind();
}

void VertexArrayBuffer::setupVertexAttribute(unsigned int index,
                                             int size,
                                             int stride,
                                             int offset) {
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE,
                          stride, (void*)(offset * sizeof(float)));
    glEnableVertexAttribArray(index);
}