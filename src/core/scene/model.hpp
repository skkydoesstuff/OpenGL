#pragma once

#include <glm/glm.hpp>

#include <memory>

#include "core/transform.hpp"
#include "core/renderer/material.hpp"
#include "core/renderer/shader.hpp"
#include "core/renderer/mesh.hpp"

enum class DrawMode {
    Opaque,
    Transparent
};

class Model {
public:
    Transform transform;
    std::shared_ptr<Mesh> mesh;
    
    Model(std::shared_ptr<Mesh> mesh);

    std::string meshSourceName;

    void updateModelMatrix();
    glm::mat4 getModelMatrix();
    
private:
    glm::mat4 model;
};