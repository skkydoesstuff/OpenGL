#pragma once

class Shader;

class VertexArrayBuffer {
private:
  unsigned int ID;
  Shader &m_Shader;

public:
  VertexArrayBuffer(Shader &shader);
  ~VertexArrayBuffer();
  void setupVertexAttribute(unsigned int index, int size, int stride, int offset);

  void bind() const;
  void unbind() const;
  void draw(unsigned int bufferType, unsigned int vertexCount, unsigned int dataType);
};