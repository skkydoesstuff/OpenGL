#pragma once

#include <glm/glm.hpp>

class Shader;

class Light {
public:
    glm::vec3 position;
    
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;

    void upload(const Shader& s);
    
    static void reset() { lightCount = 0; }

private:
    bool firstCall = true;
    static int lightCount;
};