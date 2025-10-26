#pragma once

class VertexArray {
private:
	unsigned int VAO;

public:
	VertexArray();
	~VertexArray();

	void setVertexAttrib(unsigned int index, int size, unsigned int data_type, unsigned char normalized, long int stride, const void* offset);

	void Bind();
	void Unbind();
};
