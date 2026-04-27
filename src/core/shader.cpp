#include "shader.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>

#include "utils/fileUtils.hpp"
#include "utils/debugUtils.hpp"

const std::string exeDir = getExecutableDirectory();
const std::string assetDir = exeDir + "\\assets\\";

Shader::Shader(std::string vertShaderName, std::string fragShaderName) {
    std::string vertSrcStr = readFile(assetDir+"shaders\\"+vertShaderName);
    std::string fragSrcStr = readFile(assetDir+"shaders\\"+fragShaderName);

    const char* vertSrc = vertSrcStr.c_str();
    const char* fragSrc = fragSrcStr.c_str();

    this->pId = this->linkProgram(&vertSrc, &fragSrc);
}

Shader::~Shader() {
    glDeleteProgram(this->pId);
}

void Shader::bind() const {
    glUseProgram(this->pId);
}

void Shader::setUniformMat4(const std::string& key, glm::mat4 matrix) const {
    int uni = this->findUniform(key);
    if (uni == -1) return;
    glUniformMatrix4fv(uni, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniformMat3(const std::string& key, glm::mat3 matrix) const {
    int uni = this->findUniform(key);
    if (uni == -1) return;
    glUniformMatrix3fv(uni, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniformVec4(const std::string& key, glm::vec4 vector) const {
    int uni = this->findUniform(key);
    if (uni == -1) return;
    glUniform4fv(uni, 1, glm::value_ptr(vector));
}

void Shader::setUniformVec3(const std::string& key, glm::vec3 vector) const {
    int uni = this->findUniform(key);
    if (uni == -1) return;
    glUniform3fv(uni, 1, glm::value_ptr(vector));
}

void Shader::setUniformVec2(const std::string& key, glm::vec2 vector) const {
    int uni = this->findUniform(key);
    if (uni == -1) return;
    glUniform2fv(uni, 1, glm::value_ptr(vector));
}

void Shader::setUniformFloat(const std::string& key, float value) const {
    int uni = this->findUniform(key);
    if (uni == -1) return;
    glUniform1f(uni, value);
}

void Shader::setUniformInt(const std::string& key, int value) const {
    int uni = this->findUniform(key);
    if (uni == -1) return;
    glUniform1i(uni, value);
}

int Shader::findUniform(const std::string& key) const {
    auto it = this->uniforms.find(key);
    if (it != this->uniforms.end()) {
        return it->second;
    }

    int uni = glGetUniformLocation(this->pId, key.c_str());
    this->uniforms[key] = uni;
    return uni;
}

unsigned int Shader::compileShader(unsigned int shaderType, const char* const* src) {
    if (!src || !*src || strlen(*src) == 0) {
        DEBUG_PRINT(std::cout << "Empty shader source, aborting compile\n");
        return 0;
    }

    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, src, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE) {
        int maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
    
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
        
        DEBUG_PRINT(std::cout << "Shader Compilation Error: " << &errorLog[0] << std::endl);
    }
    
    return shader;
}

unsigned int Shader::linkProgram(const char* const* vSrc, const char* const* fSrc) {
    unsigned int pId = glCreateProgram();
    unsigned int vId = this->compileShader(GL_VERTEX_SHADER, vSrc);
    unsigned int fId = this->compileShader(GL_FRAGMENT_SHADER, fSrc);

    glAttachShader(pId, vId);
    glAttachShader(pId, fId);
    glLinkProgram(pId);

    int success;
    glGetProgramiv(pId, GL_LINK_STATUS, &success);

    if (success == GL_FALSE) {
        int maxLength = 0;
        glGetProgramiv(pId, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> errorLog(maxLength);
        glGetProgramInfoLog(pId, maxLength, &maxLength, &errorLog[0]);

        DEBUG_PRINT(std::cout << "Program linkin error: " << &errorLog[0] << std::endl);
    }

    glDetachShader(pId, vId);
    glDetachShader(pId, fId);
    glDeleteShader(vId);
    glDeleteShader(fId);

    return pId;
}

