#pragma once

#include "engine/renderer.hpp"
#include "engine/shader.hpp"
#include "engine/camera.hpp"
#include "engine/shader.hpp"

#include <string>

class Scene {
private:
  Renderer& renderer;
  Shader& shader;

public:
  Scene(Renderer& renderer, Shader& shader);
  Drawable* getDrawable(const std::string& tag);
  void draw(Camera& camera, Shader& shader);
  
  Drawable* addTriangle(const std::string& tag, const std::string& texturePath = "");
  Drawable* addQuad(const std::string& tag, const std::string& texturePath = "");
};