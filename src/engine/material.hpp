#pragma once
#include "shader.hpp"

class Material {
public:
    Shader* shader = nullptr;
    unsigned int diffuseTex = 0;
    
};