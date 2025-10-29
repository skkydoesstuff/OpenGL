#include <glad/glad.h>

#include "arrayBuffer.hpp"

#include <cstdlib>

ArrayBuffer::ArrayBuffer(unsigned int type, const void* data, size_t size) {
	glGenBuffers(1, &buffer);
	glBindBuffer(type, buffer);
	glBufferData(type, size, data, GL_STATIC_DRAW);
	glBindBuffer(type, 0);

	m_Type = type;
}

ArrayBuffer::~ArrayBuffer() {
	glDeleteBuffers(1, &buffer);
}

void ArrayBuffer::Bind() {
	glBindBuffer(m_Type, buffer);
}

void ArrayBuffer::Unbind() {
	glBindBuffer(m_Type, 0);
}

void ArrayBuffer::Draw(unsigned int mode, unsigned int count, unsigned int data_type) {
  if (m_Type == GL_ARRAY_BUFFER) {
    glDrawArrays(mode, 1, count);
  } else if (m_Type == GL_ELEMENT_ARRAY_BUFFER) {
    glDrawElements(mode, count, data_type, nullptr);
  }
}