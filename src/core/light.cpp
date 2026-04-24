#include "core/light.hpp"

#include "core/shader.hpp"

int Light::lightCount = 0;

void Light::upload(const Shader& s) {
    std::string base = "lights[" + std::to_string(lightCount) + "].";
    s.setUniformVec3(base + "position",  this->position);
    s.setUniformVec3(base + "ambient",   this->ambient);
    s.setUniformVec3(base + "diffuse",   this->diffuse);
    s.setUniformVec3(base + "specular",  this->specular);
    s.setUniformFloat(base + "constant",  this->constant);
    s.setUniformFloat(base + "linear",    this->linear);
    s.setUniformFloat(base + "quadratic", this->quadratic);
    lightCount++;
    s.setUniformInt("uNumLights", lightCount);
}