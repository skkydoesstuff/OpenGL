#include <GLAD/glad.h>
#include <SDL3/sdl.h>

#include "shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define INITIAL_BUFFER_SIZE 1024

char* readShader(const char* path) {
    const char* basePath = SDL_GetBasePath();
    size_t pathLen = strlen(basePath) + strlen("/resources/shaders/") + strlen(path) + 1;
    char* finalPath = malloc(pathLen);
    snprintf(finalPath, pathLen, "%s/resources/shaders/%s", basePath, path);
    
    FILE *file = fopen(finalPath, "r");
    if (file == NULL) {
        exit(EXIT_FAILURE);
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

unsigned int compileShader(const char* const* source, unsigned int type) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, source, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        int len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        char* log = malloc(len);
        glGetShaderInfoLog(shader, len, NULL, log);
        fprintf(stderr, "Shader compile error:\n%s\n", log);
        free(log);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}


unsigned int createShaderProgram(const char* const* vertSource, const char* const* fragSource) {
    unsigned int vertexShader = compileShader(vertSource, GL_VERTEX_SHADER);
    unsigned int fragmentShader = compileShader(fragSource, GL_FRAGMENT_SHADER);

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    int log_length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);

    if (log_length > 0) {
        GLchar *info_log = (GLchar *)malloc(log_length * sizeof(GLchar));
        glGetProgramInfoLog(program, log_length, NULL, info_log);
        fprintf(stderr, "Shader Compilation Error: %s\n", info_log);
        free(info_log);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}