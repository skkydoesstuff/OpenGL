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
