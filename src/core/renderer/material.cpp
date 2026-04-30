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
        shader.setUniformBool("uHasSpecularMap", true);
    } else {
        shader.setUniformBool("uHasSpecularMap", false);
    }

    // 

    if (normal) {
        normal->bind(3);
        shader.setUniformInt("uNormalMap", 3);
        shader.setUniformBool("uHasNormalMap", true);
    } else {
        shader.setUniformBool("uHasNormalMap", false);
    }

    shader.setUniformFloat("uShininess", shininess);
    shader.setUniformFloat("uOpacity", opacityValue);
}