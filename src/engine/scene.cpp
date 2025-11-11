#include <glm/glm.hpp>

#include "engine/scene.hpp"
#include "engine/camera.hpp"
#include "engine/drawable.hpp"
#include "engine/renderer.hpp"
#include "engine/shader.hpp"

#include <string>
#include <iostream>

Scene::Scene(Renderer& renderer, Shader& shader): renderer(renderer), shader(shader) {}

Drawable* Scene::addTriangle(const std::string& tag, const std::string& texturePath) {
  const float vertices[15] = {
    // pos.x  pos.y  pos.z   u    v
    0.0f,  0.5f,  0.0f,   0.5f, 1.0f,
    -0.5f, -0.5f,  0.0f,   0.0f, 0.0f,
    0.5f, -0.5f,  0.0f,   1.0f, 0.0f
  };
  
  const unsigned int indices[3] = {
    0, 1, 2
  };

  std::unique_ptr<Drawable> t = std::make_unique<Drawable> (shader, vertices, 15, indices, 3);
  t->setScale(glm::vec3(0.01f));
  t->updateModelMatrix();

  if (!texturePath.empty()) {
    t->addTexture(texturePath);
  }

  renderer.add(tag, std::move(t));

  return renderer.getDrawable(tag);
}

Drawable* Scene::addQuad(const std::string& tag, const std::string& texturePath) {
  const float vertices[20] = {
  // pos.x  pos.y  pos.z   u    v
    -0.5f,  0.5f, 0.0f,     0.0f, 1.0f,  // top-left
    0.5f,  0.5f, 0.0f,     1.0f, 1.0f,  // top-right
    0.5f, -0.5f, 0.0f,     1.0f, 0.0f,  // bottom-right
    -0.5f, -0.5f, 0.0f,     0.0f, 0.0f   // bottom-left
  };

  const unsigned int indices[6] = {
    0, 1, 2,  // first triangle
    0, 2, 3   // second triangle
  };

  std::unique_ptr<Drawable> q = std::make_unique<Drawable> (shader, vertices, 20, indices, 6);
  q->setScale(glm::vec3(0.01f));
  q->updateModelMatrix();

  if (!texturePath.empty()) {
    q->addTexture(texturePath);
  }

  renderer.add(tag, std::move(q));

  return renderer.getDrawable(tag);
}

Drawable* Scene::getDrawable(const std::string& tag) {
  Drawable* d = renderer.getDrawable(tag);
  if (!d) {
    std::cerr << "Invalid drawable tag: " << tag << std::endl;
    return nullptr;
  }
  return d;
}

void Scene::draw(Camera& camera, Shader& shader) {
  renderer.drawAll(camera, shader);
}