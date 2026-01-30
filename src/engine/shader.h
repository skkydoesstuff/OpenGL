#pragma once

char* readShader(const char* path);

unsigned int compileShader(const char* const* source, unsigned int shaderType);
unsigned int createShaderProgram(const char* const* vertSource, const char* const* fragSource);