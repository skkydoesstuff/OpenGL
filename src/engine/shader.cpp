#include <GLAD/glad.h>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <unordered_map>
#include <string>
#include <stdexcept>
#include <fstream>

#include "shader.hpp"

unsigned int Shader::compileShader(const char* source, GLenum type) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    return shader;
}

unsigned int Shader::getLocation(const std::string& name) {
    if (locations.find(name) != locations.end()) {
        return locations.at(name);
    }

    unsigned int loc = glGetUniformLocation(program, name.c_str());
    locations.insert({name, loc});
    return loc;
}

std::string Shader::readShader(const char* path) {
    const char* basePath = SDL_GetBasePath();
    std::string finalPath = std::string(basePath) + "/resources/shaders/" + path;
    
    std::ifstream file(finalPath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open shader file: " + finalPath);
    }

    std::string result;
    std::string line;
    while (std::getline(file, line)) {
        result += line + "\n";
    }

    return result;
}

Shader::Shader(const char* vertexResourcePath, const char* fragmentResourcePath) {
    std::string vertSource = readShader(vertexResourcePath);
    std::string fragSource = readShader(fragmentResourcePath);

    const char* vertSrc = vertSource.c_str();
    const char* fragSrc = fragSource.c_str();

    unsigned int vertexShader = compileShader(vertSrc, GL_VERTEX_SHADER);        
    unsigned int fragmentShader = compileShader(fragSrc, GL_FRAGMENT_SHADER);
    
    this->program = glCreateProgram();
    glAttachShader(this->program, vertexShader);
    glAttachShader(this->program, fragmentShader);
    glLinkProgram(this->program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(this->program);
}

void Shader::Bind() {
    glUseProgram(this->program);
}

void Shader::setMat4f(glm::mat4 mat, const std::string& name) {
    unsigned int loc = getLocation(name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setInt(int val, const std::string& name) {
    unsigned int loc = getLocation(name);
    glUniform1i(loc, val);
}

void Shader::setVec3(glm::vec3 vec, const std::string& name) {
    unsigned int loc = getLocation(name);
    glUniform3fv(loc, 1, glm::value_ptr(vec));
}

void Shader::setVec4(glm::vec4 vec, const std::string& name) {
    unsigned int loc = getLocation(name);
    glUniform4fv(loc, 1, glm::value_ptr(vec));
}