#pragma once

#include <glm/glm.hpp>

class Shader;

class Light {
public:
    glm::vec3 position;
    
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    
    void upload(const Shader& s);
};