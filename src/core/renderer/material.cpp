#include "core/renderer/material.hpp"

#include <glad/glad.h>

#include "core/renderer/texture.hpp"
#include "core/renderer/shader.hpp"

void Material::bind(const Shader& shader) const {
    if (diffuse) {
        diffuse->bind(0);
        shader.setUniformInt("material.diffuse", 0);
    }

    if (specular) {
        specular->bind(1);
        shader.setUniformInt("material.specular", 1);
    }

    shader.setUniformFloat("material.shininess", shininess);
}