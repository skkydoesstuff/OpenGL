#pragma once

#include "core/renderer/shader.hpp"
#include "core/scene/camera.hpp"

#include <memory>

struct RenderContext {
    std::shared_ptr<Shader> shader;
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec3 cameraPosition;
};