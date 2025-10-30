#pragma once

#include <string>

class Shader;

class Texture {
private:
  unsigned int id;
  int width, height, channels;
  unsigned int m_Unit;
public:
  Texture(const std::string &path, Shader &shader, unsigned int unit, bool generateMipmaps = true);

  ~Texture();

  Texture &operator=(Texture &&other) noexcept;

  void bind() const;

  void unbind() const;

  unsigned int getId() const;
  int getWidth() const;
  int getHeight() const;
};