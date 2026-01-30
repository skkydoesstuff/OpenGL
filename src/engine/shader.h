#pragma once

#define MAX_UNIFORMS 32

typedef struct shader {
    unsigned int program;
    struct {
        const char* name;
        int location;
    } uniforms[32];
    unsigned int uniform_count;
} shader;


shader shader_create(const char* vertPath, const char* fragPath);
void shader_use(const shader* s);
void shader_destroy(shader* s);

void shader_setMat4f(shader* s, const char* name, const float* mat);