#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include "application/constants.hpp"
#include "application/application.hpp"

#include "engine/utils.hpp"
#include "engine/window.hpp"
#include "engine/renderer.hpp"
#include "engine/drawable.hpp"
#include "engine/camera.hpp"

#include <memory>

Application::Application():
window(WIDTH, HEIGHT, TITLE),
shader(getExecutableDir() + "/assets/shaders/shader.vs", getExecutableDir() + "/assets/shaders/shader.fs"),
camera(glm::vec3(0.0f, 0.0f, 0.0f)),
tex(getExecutableDir() + "/assets/textures/pubert.png", true) 
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

// gets called once at runtime
void Application::start() {

}

// gets called every frame
void Application::update() {
  Drawable* t = renderer.getDrawable("triangle");
  if (!t) return;
  
  glm::vec3 oldPos = t->position;
  glm::vec3 newPos = glm::vec3(0.0f, 0.0f, 1.0f * window.getDeltaTime()) + oldPos;

  t->setPosition(newPos);
}

// gets called once to setup geometry
void Application::setupGeometry() {
  const float vertices[15] = {
    // pos.x  pos.y  pos.z   u    v
    0.0f,  0.5f,  0.0f,   0.5f, 1.0f,
    -0.5f, -0.5f,  0.0f,   0.0f, 0.0f,
    0.5f, -0.5f,  0.0f,   1.0f, 0.0f
  };
  
  const unsigned int indices[3] = {
    0, 1, 2
  };

  std::unique_ptr<Drawable> triangle = std::make_unique<Drawable> (shader, vertices, 15, indices, 3);
  triangle->setScale(glm::vec3(0.01f));
  triangle->updateModelMatrix();

  camera.moveForward(-1.0f);

  renderer.add("triangle", std::move(triangle));
}

// gets called every frame with priority
void Application::render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shader.bind();
  tex.bind();
  shader.setInt("texture0", 0);
  shader.unbind();

  renderer.drawAll(camera, shader);
  window.update();
}

void Application::run() {
  start();
  setupGeometry();
  while (!window.shouldClose()) {
    render();
    update();
  }
}