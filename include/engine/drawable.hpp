#pragma once

#include <glm/glm.hpp>

#include "engine/shader.hpp"
#include "engine/vertexArrayBuffer.hpp"
#include "engine/arrayBuffer.hpp"
#include "engine/camera.hpp"

#include <stddef.h>

class Drawable {
private:
  Shader& shader;
  VertexArrayBuffer VAO;
  ArrayBuffer VBO;
  ArrayBuffer EBO;
  unsigned int vertexCount;

public:
  Drawable(Shader& shader, const float* vertices, const size_t vertexCount, const unsigned int* indices, size_t indexCount);

  void setScale(glm::vec3 s);
  void setRotation(const float angleDeg, glm::vec3 axis);
  void setPosition(glm::vec3 pos);

  void updateModelMatrix();

  void draw(const Camera& camera, Shader& shader) const;

  glm::mat4 model = glm::mat4(1.0f);
  glm::vec3 position{0.0f};
  glm::vec3 scale{1.0f};
  glm::vec3 rotationAxis{0.0f, 1.0f, 0.0f};
  float rotationAngle{0.0f};
};