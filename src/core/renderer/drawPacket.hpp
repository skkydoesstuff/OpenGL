#pragma once

#include "core/renderer/mesh.hpp"
#include "core/renderer/material.hpp"

#include <glm/glm.hpp>

struct DrawPacket {
    std::shared_ptr<Mesh> mesh;
    int submeshIndex;

    std::shared_ptr<Material> material;
    glm::mat4 model;

    float opacity;
};