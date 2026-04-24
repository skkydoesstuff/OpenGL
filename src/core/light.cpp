#include "core/light.hpp"

#include "core/shader.hpp"

void Light::upload(const Shader& s) {
    s.bind();
    s.setUniformVec3("light.position", this->position);
    s.setUniformVec3("light.ambient", this->ambient);
    s.setUniformVec3("light.diffuse", this->diffuse);
    s.setUniformVec3("light.specular", this->specular);
}