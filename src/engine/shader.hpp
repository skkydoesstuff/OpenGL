#pragma once

#include <GLAD/glad.h>
#include <glm/glm.hpp>

#include <unordered_map>
#include <string>

class Shader {
private:
    unsigned int program;
    std::unordered_map<std::string, unsigned int> locations;

    unsigned int compileShader(const char* source, GLenum type);
    unsigned int getLocation(const std::string& name);

    std::string readShader(const char* path);

public:
    Shader(const char* vertexResourcePath, const char* fragmentResourcePath);
    ~Shader();
    
    void Bind();
    void setMat4f(glm::mat4 mat, const std::string& name);
    void setInt(int val, const std::string& name);
    void setVec3(glm::vec3 vec, const std::string& name);
    void setVec4(glm::vec4 vec, const std::string& name);
};