#include <GLAD/glad.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "entity.hpp"
#include "mesh.hpp"
#include "material.hpp"

void Entity::updateModelMatrix() {
    this->model = glm::mat4(1.0f);
    this->model = glm::translate(this->model, this->transform.position);
    this->model = glm::rotate(this->model, this->transform.rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));
    this->model = glm::rotate(this->model, this->transform.rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
    this->model = glm::rotate(this->model, this->transform.rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));
    this->model = glm::scale(this->model, this->transform.scale);
}

Entity::Entity(Mesh* mesh, Material* mat) {
    this->mesh = mesh;
    this->transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    this->transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);

    this->material = mat;
}

void Entity::Render() {
    Shader* s = this->material->shader;
    s->Bind();

    this->updateModelMatrix();

    s->setMat4f(this->model, "model");

    if (this->material->diffuseTex) {
        this->material->Bind();
    }

    this->mesh->Draw();
}