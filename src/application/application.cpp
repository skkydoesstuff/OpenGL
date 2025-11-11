#include <glm/glm.hpp>
#include <stb_image.h>

#include "application/application.hpp"

#include "engine/utils.hpp"
#include "engine/window.hpp"
#include "engine/renderer.hpp"
#include "engine/camera.hpp"


Application::Application():
window(WIDTH, HEIGHT, TITLE),
shader(getExecutableDir() + "/assets/shaders/shader.vs", getExecutableDir() + "/assets/shaders/shader.fs"),
camera(glm::vec3(0.0f, 0.0f, 0.0f)),
scene(renderer, shader)
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

// gets called once at runtime
void Application::start() {

}

// gets called every frame
void Application::update() {
  Drawable* q = renderer.getDrawable("quad");
  if (!q) return;

  glm::vec3 oldPos = q->position;
  glm::vec3 newPos = glm::vec3(0.0f, 0.0f, 1.0f * window.getDeltaTime()) + oldPos;

  q->setPosition(newPos);
  q->updateModelMatrix();
}

// gets called once to setup geometry
void Application::setupGeometry() {
  Drawable* a = scene.addQuad("quad", getExecutableDir() + "/assets/textures/pubert.png");
  a->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

  camera.moveForward(-1.0f);
}

// gets called every frame with priority
void Application::render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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