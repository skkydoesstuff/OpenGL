#include <glad/glad.h>
#include <stb_image.h>

#include "texture.hpp"

#include <string>
#include <iostream>

Texture::Texture(const std::string &path, bool generateMipmaps) {
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);

  // Set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  generateMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Load image
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
  if (data) {
    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);

    if (generateMipmaps) {
      glGenerateMipmap(GL_TEXTURE_2D);
    }
  } else {
    std::cerr << "Failed to load texture: " << path << std::endl;
  }
  stbi_image_free(data);
}

Texture::~Texture() { glDeleteTextures(1, &id); }

void Texture::bind(unsigned int unit) const {
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

unsigned int Texture::getId() const { return id; }
int Texture::getWidth() const { return width; }
int Texture::getHeight() const { return height; }