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
    
    Entity(Mesh* mesh, Material* mat);
    void Render();
};