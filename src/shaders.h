#pragma once
#include <GLAD/glad.h>

char* readShader(const char* path);
unsigned int compileShader(const char* shaderSource, GLenum type);
unsigned int createShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);