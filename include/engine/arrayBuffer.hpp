#pragma once

class ArrayBuffer {
private:
  unsigned int ID;
  unsigned int bufferType;

public:
  ArrayBuffer(unsigned int bufferType, signed long int dataSize, const void* data);
  ~ArrayBuffer();

  void bind();
  void unbind();
};