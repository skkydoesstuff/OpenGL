#pragma once

#include "core/texture.hpp"
#include "core/shader.hpp"

#include <memory>

class Material {
public:
    std::shared_ptr<Texture> diffuse;
    std::shared_ptr<Texture> specular;
    float shininess = 32.0f;

    void bind(const Shader& shader) const;
}; 