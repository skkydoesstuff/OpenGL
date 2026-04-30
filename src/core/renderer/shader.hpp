#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <unordered_map>
#include <string>

class Shader {
public:
    Shader(std::string vertShaderName, std::string fragShaderName);
    ~Shader();

    Shader(const Shader &) = delete;
    Shader& operator=(const Shader &) = delete;

    void bind() const;    
    void setUniformMat4(const std::string& key, glm::mat4 matrix) const;
    void setUniformMat3(const std::string& key, glm::mat3 matrix) const;
    void setUniformVec4(const std::string& key, glm::vec4 vector) const;
    void setUniformVec3(const std::string& key, glm::vec3 vector) const;
    void setUniformVec2(const std::string& key, glm::vec2 vector) const;
    void setUniformFloat(const std::string& key, float value) const;
    void setUniformInt(const std::string& key, int value) const;
    void setUniformBool(const std::string& key, bool value) const;
    
private:
    unsigned int pId;

    mutable std::unordered_map<std::string, int> uniforms;

    int findUniform(const std::string& key) const;
    
    unsigned int compileShader(unsigned int shaderType, const char* const* src);
    unsigned int linkProgram(const char* const* vSrc, const char* const* fSrc);
};

