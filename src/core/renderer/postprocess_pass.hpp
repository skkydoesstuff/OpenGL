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
    bool useBrightBuffer = false;

    // if set, save the output of this pass under this name
    std::string saveOutputAs;

    // named snapshots to bind as extra textures: uniform name -> snapshot name
    std::unordered_map<std::string, std::string> extraTextures;

    void applyUniforms() const {
        for (auto& [name, value] : uniforms) {
            std::visit([&](auto&& v) {
                using T = std::decay_t<decltype(v)>;
                if constexpr (std::is_same_v<T, int>)            shader->setUniformInt(name, v);
                else if constexpr (std::is_same_v<T, float>)     shader->setUniformFloat(name, v);
                else if constexpr (std::is_same_v<T, glm::vec2>) shader->setUniformVec2(name, v);
                else if constexpr (std::is_same_v<T, glm::vec3>) shader->setUniformVec3(name, v);
                else if constexpr (std::is_same_v<T, glm::vec4>) shader->setUniformVec4(name, v);
                else if constexpr (std::is_same_v<T, glm::mat4>) shader->setUniformMat4(name, v);
            }, value);
        }
    }
};