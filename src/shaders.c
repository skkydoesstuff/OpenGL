#include <GLAD/glad.h>
#include <SDL3/SDL.h>

#include "shaders.h"
#include "error_handling.h"

#include <string.h>
#include <stdlib.h>

#define INITIAL_BUFFER_SIZE 1024

char* readShader(const char* path) {
    const char* basePath = SDL_GetBasePath();
    size_t pathLen = strlen(basePath) + strlen("/resources/shaders/") + strlen(path) + 1;
    char* finalPath = malloc(pathLen);
    snprintf(finalPath, pathLen, "%s/resources/shaders/%s", basePath, path);
    
    FILE *file = fopen(finalPath, "r");
    if (file == NULL) {
        throw_error(strcat("Could not open file: ", finalPath));
        free(finalPath);
        return NULL;
    }

    char* result = malloc(INITIAL_BUFFER_SIZE);
    size_t resultSize = INITIAL_BUFFER_SIZE;
    size_t resultLen = 0;
    char buffer[INITIAL_BUFFER_SIZE];

    while (fgets(buffer, INITIAL_BUFFER_SIZE, file) != NULL) {
        size_t len = strlen(buffer);
        while (resultLen + len >= resultSize) {
            resultSize *= 2;
            result = realloc(result, resultSize);
        }
        strcpy(result + resultLen, buffer);
        resultLen += len;
    }

    fclose(file);
    free(finalPath);
    return result;
}

unsigned int compileShader(const char* shaderSource, GLenum type) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        throw_error(strcat("failed to compile shader: ", infoLog));
    }
    return shader;
}

unsigned int createShaderProgram(unsigned int vertexShader, unsigned int fragmentShader) {
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        fprintf(stderr, "ERROR: Shader program linking failed:\n%s\n", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}