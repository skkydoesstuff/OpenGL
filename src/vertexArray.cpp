#include <glad/glad.h>

#include "vertexArray.hpp"

VertexArray::VertexArray() {
	glGenVertexArrays(1, &VAO);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &VAO);
}

void VertexArray::setVertexAttrib(unsigned int index, int size, unsigned int data_type, unsigned char normalized, long int stride, const void* offset) {
	glVertexAttribPointer(index, size, data_type, normalized, stride, offset);
	glEnableVertexAttribArray(index);
}

void VertexArray::Bind() {
	glBindVertexArray(VAO);
}

void VertexArray::Unbind() {
	glBindVertexArray(0);
}
