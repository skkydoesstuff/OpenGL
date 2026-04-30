#pragma once

#include "core/renderer/texture.hpp"
#include "core/renderer/shader.hpp"

#include <memory>

class Material {
public:
    std::shared_ptr<Texture> diffuse;
    std::shared_ptr<Texture> specular;
    std::shared_ptr<Texture> opacity;
    bool hasOpacityMap = false;
    
    std::shared_ptr<Texture> normal;
    bool hasNormalMap = false;

    float shininess = 32.0f;
    float opacityValue = 1.0f;

    void bind(const Shader& shader) const;
}; 