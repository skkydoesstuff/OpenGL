#include <glad/glad.h>

#include <engine/shader.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

const char* readFile(const std::string& path) {
    static std::string buffer;
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file\n";
        return nullptr;
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    buffer = ss.str();
    return buffer.c_str();
}

unsigned int createShader(const std::string& shaderSourceFilePath, GLenum type) {
    const char* src = readFile(shaderSourceFilePath);

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint logLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);

        std::string log(logLen, '\0');
        glGetShaderInfoLog(shader, logLen, nullptr, log.data());
        std::cerr << "Shader compilation failed (" << shaderSourceFilePath << "):\n"
                  << log << std::endl;

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

unsigned int createProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLint logLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);

        std::string log(logLen, '\0');
        glGetProgramInfoLog(program, logLen, nullptr, log.data());
        std::cerr << "Program linking failed:\n" << log << std::endl;

        glDeleteProgram(program);
        return 0;
    }

    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if (!success) {
        GLint logLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);

        std::string log(logLen, '\0');
        glGetProgramInfoLog(program, logLen, nullptr, log.data());
        std::cerr << "Program validation failed:\n" << log << std::endl;
    }

    return program;
}

Shader::Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath) {
  vertexShaderID = createShader(vertexFilePath, GL_VERTEX_SHADER);
  fragmentShaderID = createShader(fragmentFilePath, GL_FRAGMENT_SHADER);
  programID = createProgram(vertexShaderID, fragmentShaderID);
}

Shader::~Shader() {
  glDetachShader(programID, vertexShaderID);
  glDetachShader(programID, fragmentShaderID);
  glDeleteShader(vertexShaderID);
  glDeleteShader(fragmentShaderID);
  glDeleteProgram(programID);
}

void Shader::bind() {
  glUseProgram(programID);
}

void Shader::unbind() {
  glUseProgram(0);
}

unsigned int Shader::getUniformLocation(const std::string& name) {
  std::unordered_map<std::string, unsigned int>::iterator potentialLoc = uniformLocations.find(name);

  if (potentialLoc != uniformLocations.end()) {
    return potentialLoc->second;
  }

  int loc = glGetUniformLocation(programID, name.c_str());
  uniformLocations[name] = loc;
  return loc;
}