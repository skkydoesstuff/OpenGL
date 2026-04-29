#include "core/renderer/material.hpp"

#include <glad/glad.h>

#include "core/renderer/texture.hpp"
#include "core/renderer/shader.hpp"

void Material::bind(const Shader& shader) const {
    // Diffuse (required)
    if (diffuse) {
        diffuse->bind(0);
        shader.setUniformInt("uDiffuseMap", 0);
    }

    // Specular (optional)
    if (specular) {
        specular->bind(1);
        shader.setUniformInt("uSpecularMap", 1);
        shader.setUniformFloat("uHasSpecularMap", 1.0f);
    } else {
        shader.setUniformFloat("uHasSpecularMap", 0.0f);
    }

    shader.setUniformFloat("uShininess", shininess);
    shader.setUniformFloat("uOpacity", opacityValue);
}