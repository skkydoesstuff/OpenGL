#include "core/scene/light.hpp"

#include "core/renderer/shader.hpp"

void Light::upload(const Shader& s, int index) {
    std::string base = "lights[" + std::to_string(index) + "].";
    s.setUniformVec3(base + "position",  this->position);
    s.setUniformVec3(base + "ambient",   this->ambient);
    s.setUniformVec3(base + "diffuse",   this->diffuse);
    s.setUniformVec3(base + "specular",  this->specular);
    s.setUniformFloat(base + "constant",  this->constant);
    s.setUniformFloat(base + "linear",    this->linear);
    s.setUniformFloat(base + "quadratic", this->quadratic);
}