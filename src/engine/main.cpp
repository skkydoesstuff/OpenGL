#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "application/constants.hpp"
#include "engine/window.hpp"
#include "engine/vertexArrayBuffer.hpp"
#include "engine/arrayBuffer.hpp"
#include "engine/utils.hpp"
#include "engine/shader.hpp"

#include <string>

int main() {
  Window window(WIDTH, HEIGHT, TITLE);

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  constexpr float vertices[9] = {
     0.0f,  0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f
  };

  constexpr unsigned int indices[3] = {
    0, 1, 2
  };

  std::string exeDir = getExecutableDir();
  Shader shader(exeDir + "/assets/shaders/shader.vs", exeDir + "/assets/shaders/shader.fs");

  VertexArrayBuffer VAO(shader);
  VAO.bind();

  ArrayBuffer VBO(GL_ARRAY_BUFFER, sizeof(vertices), vertices);
  VBO.bind();

  ArrayBuffer EBO(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices);
  EBO.bind();

  VAO.setupVertexAttribute(0, 3, 3 * sizeof(float), 0);

  VBO.unbind();
  VAO.unbind();

  while (!window.shouldClose()) {
    glClear(GL_COLOR_BUFFER_BIT);

    VAO.draw(GL_ELEMENT_ARRAY_BUFFER, 3, GL_UNSIGNED_INT);

    window.update();
  }
}