#pragma once

#include <memory>

class Renderer;
class Shader;
class Drawable;

class Primative {
private:
  Renderer& renderer;
  Shader& shader;

  std::unique_ptr<Drawable> drawable;

public:
  Primative(Renderer& renderer, Shader& shader);

  void createTriangle();

};