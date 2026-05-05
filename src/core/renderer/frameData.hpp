#pragma once

#include "core/renderer/mesh.hpp"
#include "core/renderer/material.hpp"
#include "core/renderer/renderContext.hpp"
#include "core/scene/objects/light.hpp"

struct DrawCommand {
    Mesh* mesh;
    const SubMesh* submesh;
    std::shared_ptr<Material> material;
    glm::mat4 model;

    float depth = 0.0f;        // only meaningful for transparent
    uint8_t flags = 0;      // opaque/transparent/etc
};

enum DrawFlags : uint8_t {
    Opaque      = 1 << 0,
    Transparent = 1 << 1
};

struct FrameData {
    std::vector<DrawCommand> commands;
    std::vector<Light*> lights;
};

struct FrameSnapshot {
    FrameData frame;
    RenderContext ctx;
};
