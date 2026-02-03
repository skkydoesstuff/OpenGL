#pragma once

#include <glm/glm.hpp>
#include "shader.hpp"
#include "mesh.hpp"
#include "material.hpp"

typedef struct Transform {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
} Transform;

class Entity {
private:
    glm::mat4 model;
    Mesh* mesh;
    Material* material = nullptr;

    void updateModelMatrix();

public:
    Transform transform;
    glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);
    float opacity = 1.0f;
    
    Entity(Mesh* mesh, Material* mat);
    void Render();
};