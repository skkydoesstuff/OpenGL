#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/drawable.hpp"
#include "engine/shader.hpp"
#include "engine/vertexArrayBuffer.hpp"
#include "engine/arrayBuffer.hpp"

#include "application/constants.hpp"

Drawable::Drawable(
  Shader& shader,
  const float* vertices,
  const size_t vertexCount,
  const unsigned int* indices,
  const size_t indexCount
)
  : shader(shader),
    VAO(shader),
    VBO(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertices),
    EBO(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices),
    vertexCount(vertexCount)
{
  VAO.bind();
  VBO.bind();
  EBO.bind();
  VAO.setupVertexAttribute(0, 3, 5 * sizeof(float), 0);
  VAO.setupVertexAttribute(1, 2, 5 * sizeof(float), 3);
  VBO.unbind();
  VAO.unbind();
}

void Drawable::addTexture(const std::string& path) {
  textures.emplace_back(path, true);
}

void Drawable::setScale(glm::vec3 s) {
  scale = s;
}

void Drawable::setRotation(const float angleDeg, glm::vec3 axis) {
  rotationAngle = angleDeg;
  rotationAxis = axis;
} 

void Drawable::setPosition(glm::vec3 pos) {
  position = pos;
}

void Drawable::updateModelMatrix() {
  model = glm::mat4(1.0f);
  model = glm::translate(model, position);
  model = glm::rotate(model, glm::radians(rotationAngle), rotationAxis);
  model = glm::scale(model, scale);
}

void Drawable::draw(const Camera& camera, Shader& shader) const {
  shader.bind();
  shader.setMat4("model", model);
  shader.setMat4("view", camera.getViewMatrix());
  shader.setMat4("projection", camera.getProjection((float) WIDTH/HEIGHT));

  for (size_t i = 0; i < textures.size(); i++) {
    textures[i].bind(i);
    std::string name = "texture" + std::to_string(i);
    shader.setInt(name, i);
  }

  VAO.bind();
  glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, nullptr);
  VAO.unbind();
  shader.unbind();
}
