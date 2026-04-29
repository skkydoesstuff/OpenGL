#pragma once

#include "core/renderer/texture.hpp"
#include "core/renderer/shader.hpp"

#include <memory>

class Material {
public:
    std::shared_ptr<Texture> diffuse;
    std::shared_ptr<Texture> specular;
    float shininess = 32.0f;
    float opacity = 1.0f;

    void bind(const Shader& shader) const;
}; 