#include "application/constants.hpp"
#include "application/application.hpp"

#include "engine/utils.hpp"
#include "engine/window.hpp"
#include "engine/renderer.hpp"
#include "engine/drawable.hpp"

Application::Application():
window(WIDTH, HEIGHT, TITLE),
shader(getExecutableDir() + "/assets/shaders/shader.vs", getExecutableDir() + "/assets/shaders/shader.fs")  {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void Application::run() {
  const float vertices[9] = {
    0.0f, 0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f
  };
  
  const unsigned int indices[3] = {
    0, 1, 2
  };

  Drawable d(shader, vertices, 9, indices, 3);
  renderer.add(d);

  while (!window.shouldClose()) {
    glClear(GL_COLOR_BUFFER_BIT);
    renderer.drawAll();
    window.update();
  }
}