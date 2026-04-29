#pragma once

#include "core/renderer/mesh.hpp"
#include "core/renderer/material.hpp"
#include "core/renderer/renderContext.hpp"
#include "core/scene/model.hpp"
#include "core/scene/light.hpp"

struct TransparentDrawItem {
    Mesh* mesh;
    const SubMesh* submesh;
    std::shared_ptr<Material> material;
    glm::mat4 model;
    float distance;
};

struct OpaqueDrawItem {
    Mesh* mesh;
    const SubMesh* submesh;
    std::shared_ptr<Material> material;
    glm::mat4 model;
};

struct FrameData {
    std::vector<Model*> models;
    std::vector<Light*> lights;

    std::vector<TransparentDrawItem> transparent;
    std::vector<OpaqueDrawItem> opaque;
};

struct FrameSnapshot {
    FrameData frame;
    RenderContext ctx;
};