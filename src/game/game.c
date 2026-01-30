#include <GLAD/glad.h>
#include <SDL3/SDL.h>
#include <cglm/cglm.h>

#include "game.h"
#include "shader.h"
#include "mesh.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct entity {
    mesh* mesh;
    vec3 position;
    vec3 rotation;
    vec3 scale;

    mat4 model;
} entity; 

void zeroEntity(entity* e) {
    glm_vec3_zero(e->position);
    glm_vec3_zero(e->rotation);
    e->scale[0] = 1.0f;
    e->scale[1] = 1.0f;
    e->scale[2] = 1.0f;
}

void constructModel(entity* e) {
    glm_mat4_identity(e->model);
    glm_translate(e->model, e->position);
    glm_rotate(e->model, e->rotation[0], (vec3){1.0f, 0.0f, 0.0f});
    glm_rotate(e->model, e->rotation[1], (vec3){0.0f, 1.0f, 0.0f});
    glm_rotate(e->model, e->rotation[2], (vec3){0.0f, 0.0f, 1.0f});
    glm_scale(e->model, e->scale);
}

mesh triangle = {0};
entity triangle_e = {0};
shader shaderObj = {0};

void setup() {
    float vertices[9] = {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };
    
    shaderObj = shader_create("shader.vert", "shader.frag");
    shader_use(&shaderObj);

    mesh_init(&triangle, vertices, (sizeof(vertices) / sizeof(vertices[0])), NULL, 0);

    zeroEntity(&triangle_e);
    triangle_e.mesh = &triangle;

    mat4 view;
    glm_lookat(
        (vec3) {0.0f, 0.0f, 3.0f},
        (vec3) {0.0f, 0.0f, 0.0f},
        (vec3) {0.0f, 1.0f, 0.0f},
        view
    );

    mat4 projection;
    glm_perspective(glm_rad(90.0f), 800.0f/600.0f, 0.1f, 100.f, projection);

    shader_setMat4f(&shaderObj, "view", (float*)view);
    shader_setMat4f(&shaderObj, "projection", (float*)projection);

    triangle.vertexCount = sizeof(vertices) / sizeof(float);
}

void physicsLoop(){}

void renderLoop() {
    constructModel(&triangle_e);

    triangle_e.rotation[1] += 0.1;
    shader_setMat4f(&shaderObj, "model", (float*)triangle_e.model);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void logicLoop() {

}

void cleanup() {

}