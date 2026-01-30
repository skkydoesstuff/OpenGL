#include <GLAD/glad.h>
#include <SDL3/SDL.h>
#include <cglm/cglm.h>

#include "game.h"
#include "shader.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct mesh {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int vertexCount;
    unsigned int indexCount;

    mat4 model;
} mesh;

typedef struct entity {
    mesh mesh;
    vec3 position;
    vec3 rotation;
    vec3 scale;
} entity; 

void zeroEntity(entity* e) {
    glm_vec3_zero(e->position);
    glm_vec3_zero(e->rotation);
    e->scale[0] = 1.0f;
    e->scale[1] = 1.0f;
    e->scale[2] = 1.0f;
}

void constructModel(entity* e, unsigned int shaderProgram) {
    glm_mat4_identity(e->mesh.model);
    glm_translate(e->mesh.model, e->position);
    glm_rotate(e->mesh.model, e->rotation[0], (vec3){1.0f, 0.0f, 0.0f});
    glm_rotate(e->mesh.model, e->rotation[1], (vec3){0.0f, 1.0f, 0.0f});
    glm_rotate(e->mesh.model, e->rotation[2], (vec3){0.0f, 0.0f, 1.0f});
    glm_scale(e->mesh.model, e->scale);
}

mesh triangle = {0};
entity triangle_e = {0};
unsigned int program;

void setup() {
    float vertices[9] = {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    const char *base = SDL_GetBasePath();

    const char* vertSrc = readShader("shader.vert");
    const char* fragSrc = readShader("shader.frag");
    
    program = createShaderProgram(&vertSrc, &fragSrc);
    glUseProgram(program);

    glGenVertexArrays(1, &triangle.VAO);
    glBindVertexArray(triangle.VAO);

    glGenBuffers(1, &triangle.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, triangle.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    zeroEntity(&triangle_e);
    triangle_e.mesh = triangle;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    mat4 view;
    glm_lookat(
        (vec3) {0.0f, 0.0f, 3.0f},
        (vec3) {0.0f, 0.0f, 0.0f},
        (vec3) {0.0f, 1.0f, 0.0f},
        view
    );

    mat4 projection;
    glm_perspective(glm_rad(90.0f), 800.0f/600.0f, 0.1f, 100.f, projection);

    unsigned int viewID = glGetUniformLocation(program, "view");
    unsigned int projectionID = glGetUniformLocation(program, "projection");

    glUniformMatrix4fv(viewID, 1, GL_FALSE, (float*)view);
    glUniformMatrix4fv(projectionID, 1, GL_FALSE, (float*)projection);

    triangle.vertexCount = sizeof(vertices) / sizeof(float);
}

void physicsLoop(){}

void renderLoop() {
    constructModel(&triangle_e, program);

    triangle_e.rotation[1] += 1;
    unsigned int modelID = glGetUniformLocation(program, "model");
    glUniformMatrix4fv(modelID, 1, GL_FALSE, (float*) triangle_e.mesh.model);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void logicLoop() {

}
