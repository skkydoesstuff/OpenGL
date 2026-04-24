#pragma once

#include <memory>
#include <unordered_map>
#include <variant>
#include <string>

#include <glm/glm.hpp>
#include "core/shader.hpp"

using UniformValue = std::variant<
    int,
    float,
    glm::vec2,
    glm::vec3,
    glm::vec4,
    glm::mat4
>;

struct PostProcessPass {
    std::shared_ptr<Shader> shader;
    std::unordered_map<std::string, UniformValue> uniforms;
};