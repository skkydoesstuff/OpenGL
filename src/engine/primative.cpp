#include "engine/primative.hpp"
#include "engine/renderer.hpp"
#include "engine/drawable.hpp"


Primative::Primative(Renderer& renderer, Shader& shader): renderer(renderer), shader(shader) {};

void Primative::createTriangle() {
  const float vertices[15] = {
    // pos.x  pos.y  pos.z   u    v
    0.0f,  0.5f,  0.0f,   0.5f, 1.0f,
    -0.5f, -0.5f,  0.0f,   0.0f, 0.0f,
    0.5f, -0.5f,  0.0f,   1.0f, 0.0f
  };
  
  const unsigned int indices[3] = {
    0, 1, 2
  };

  drawable = std::make_unique<Drawable> (shader, vertices, 15, indices, 3);
  drawable->setScale(glm::vec3(0.01f));
  drawable->updateModelMatrix();

  renderer.add("triangle", std::move(drawable));
}