#pragma once

#include <cstdlib>

class ArrayBuffer {
private:
	unsigned int buffer;
	unsigned int m_Type;

public:
	ArrayBuffer(unsigned int type, const void* data, size_t size);
	~ArrayBuffer();

	void Bind();
	void Unbind();
};
