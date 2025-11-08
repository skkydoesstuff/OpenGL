#include <glad/glad.h>

#include "engine/arrayBuffer.hpp"

ArrayBuffer::ArrayBuffer(unsigned int bufferType, signed long int dataSize, const void* data) {
  this->bufferType = bufferType;
  
  glGenBuffers(1, &ID);
  bind();
  glBufferData(bufferType, dataSize, data, GL_STATIC_DRAW);
  unbind();
}

ArrayBuffer::~ArrayBuffer() {
  glDeleteBuffers(1, &ID);
}

void ArrayBuffer::bind() {
  glBindBuffer(this->bufferType, ID);
}

void ArrayBuffer::unbind() {
  glBindBuffer(bufferType, 0);
}