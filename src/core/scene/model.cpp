#include "model.hpp"

#include "core/renderer/mesh.hpp"
#include "core/renderer/shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>

Model::Model(std::shared_ptr<Shader> shader, std::shared_ptr<Mesh> mesh) {
    this->shader = std::move(shader);
    this->mesh = std::move(mesh);

    this->transform.position = glm::vec3(0.0f);
    this->transform.rotation = glm::vec3(0.0f);
    this->transform.scale = glm::vec3(1.0f);
}

void Model::updateModelMatrix() {
    this->model = glm::mat4(1.0f);
    this->model = glm::translate(this->model, this->transform.position);
    this->model = glm::rotate(this->model, glm::radians(this->transform.rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
    this->model = glm::rotate(this->model, glm::radians(this->transform.rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
    this->model = glm::rotate(this->model, glm::radians(this->transform.rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));
    this->model = glm::scale(this->model, this->transform.scale);
}

void Model::draw(
    DrawMode mode,
    std::function<std::shared_ptr<Material>(const std::string&)> getMaterial
) {
    shader->bind();
    shader->setUniformMat4("model", model);

    bool blendingEnabled = (mode == DrawMode::Transparent);

    if (blendingEnabled) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);
    } else {
        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
    }

    for (const auto& sm : mesh->submeshes) {
        auto mat = getMaterial(sm.materialName);

        if (!mat)
            continue;

        float opacity = mat->opacity;

        // filter by mode
        if (mode == DrawMode::Opaque && opacity < 1.0f)
            continue;

        if (mode == DrawMode::Transparent && opacity >= 1.0f)
            continue;

        mat->bind(*shader);
        mesh->drawSubMesh(sm);
    }

    if (blendingEnabled) {
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    }
}
