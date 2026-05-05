#include "core/renderer/material.hpp"

#include <glad/glad.h>

#include "core/renderer/texture.hpp"
#include "core/renderer/shader.hpp"


void Material::bind(const Shader& shader) const {
    if (diffuse) {
        diffuse->bind(0);
        shader.setUniformInt("uDiffuseMap", 0);
    }
    if (specular) {
        specular->bind(1);
        shader.setUniformInt("uSpecularMap", 1);
        shader.setUniformFloat("uHasSpecularMap", 1.0f);
    } else {
        shader.setUniformFloat("uHasSpecularMap", 0.0f);
    }
    if (opacityMap) {
        opacityMap->bind(2);
        shader.setUniformInt("uOpacityMap", 2);
        shader.setUniformBool("uHasCutout", hasCutoutMap);
    } else {
        shader.setUniformBool("uHasCutout", false);
    }
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