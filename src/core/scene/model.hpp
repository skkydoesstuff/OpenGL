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
    
    Model(std::shared_ptr<Mesh> mesh);
    
    void updateModelMatrix();
    glm::mat4 getModelMatrix();
    
private:
    glm::mat4 model;
};