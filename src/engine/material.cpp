#include "material.hpp"

Material::Material(Shader* shader, unsigned int diffuseTex): shader(shader), diffuseTex(diffuseTex) {}

void Material::Bind() {
    this->shader->Bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->diffuseTex);
    this->shader->setInt(0, "u_Diffuse");
}