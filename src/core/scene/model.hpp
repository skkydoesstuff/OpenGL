#pragma once

#include <glm/glm.hpp>

#include <memory>

#include "core/transform.hpp"
#include "core/renderer/material.hpp"

class Shader;
class Mesh;

class Model {
public:
    Transform transform;

    Model(std::shared_ptr<Shader> shader, std::shared_ptr<Mesh> mesh);

    void updateModelMatrix();
    void draw(
        std::function<std::shared_ptr<Material>(const std::string&)> getMaterial
    );
private:
    // use shared_ptr so you dont create copies of GPU resources
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Mesh> mesh;
    

    glm::mat4 model;
};