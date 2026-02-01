#include <GLAD/glad.h>
#include <glm/ext/matrix_transform.hpp>

#include "entity.hpp"

void Entity::updateModelMatrix() {
    this->model = glm::mat4(1.0f);
    this->model = glm::translate(this->model, this->transform.position);
    this->model = glm::rotate(this->model, this->transform.rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));
    this->model = glm::rotate(this->model, this->transform.rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
    this->model = glm::rotate(this->model, this->transform.rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));
    this->model = glm::scale(this->model, this->transform.scale);
}

Entity::Entity(Mesh* mesh, Shader* shader) {
    this->mesh = mesh;
    this->shader = shader;
    this->transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    this->transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

void Entity::draw() {
    updateModelMatrix();
    shader->setMat4f(this->model, "model");

    glBindVertexArray(this->mesh->VAO);
    
    if (this->mesh->indexCount > 0) {
        glDrawElements(GL_TRIANGLES, this->mesh->indexCount, GL_UNSIGNED_INT, (void*)0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, this->mesh->vertexCount);
    }
}