#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

  glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);  // camera at z=3
  glm::vec3 cameraTarget= glm::vec3(0.0f, 0.0f, 0.0f);  // looking at origin
  glm::vec3 up          = glm::vec3(0.0f, 1.0f, 0.0f);  // world up
  float fov             = 45.0f;                         // field of view
  float aspect          = static_cast<float>(WIDTH)/HEIGHT;
  float nearPlane       = 0.1f;
  float farPlane        = 100.0f;

  glm::mat4 view       = glm::lookAt(cameraPos, cameraTarget, up);
  glm::mat4 projection = glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
  glm::mat4 model      = glm::mat4(1.0f);

  shader.bind();
  shader.setMat4("model", model);
  shader.setMat4("view", view);
  shader.setMat4("projection", projection);

  while (!window.shouldClose()) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderer.drawAll();
    window.update();
  }
}