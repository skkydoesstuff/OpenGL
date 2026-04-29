#pragma once

#include <glm/glm.hpp>

#include <memory>

#include "core/transform.hpp"
#include "core/renderer/material.hpp"

class Shader;
class Mesh;

enum class DrawMode {
    Opaque,
    Transparent
};

class Model {
public:
    Transform transform;
    std::shared_ptr<Mesh> mesh;
    glm::mat4 model;

    Model(std::shared_ptr<Shader> shader, std::shared_ptr<Mesh> mesh);

    void updateModelMatrix();
    void draw(
        DrawMode mode,
        std::function<std::shared_ptr<Material>(const std::string&)> getMaterial
    );
private:
    std::shared_ptr<Shader> shader;
};