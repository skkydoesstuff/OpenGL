#include "core/material.hpp"

#include <glad/glad.h>

#include "core/texture.hpp"
#include "core/shader.hpp"

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