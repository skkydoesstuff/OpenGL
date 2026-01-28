#define SDL_MAIN_HANDLED

#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <cglm/cglm.h>

#include <stdio.h>
#include <stdbool.h>

#include "init.h"
#include "error_handling.h"
#include "engineState.h"
#include "cleanup.h"
#include "shaders.h"
#include "mesh.h"
#include "mvp.h"
#include "entity.h"
#include "renderer.h"

#include "defines.h"

int main(void) {
    engineState state = {0};
    initContext(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FLAGS, &state);

    float vertices[12] = {
        -0.5f, 0.5f, 0.0f, // top left
        0.5f, 0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f // bottom left
    };

    unsigned int indices[6] = {
        // this is the order the vertices connect
        0, 1, 2, // top right triangle
        2, 3, 0  // bottom left triangle
    };

    unsigned int vertexShader = compileShader(readShader("shader.vert"), GL_VERTEX_SHADER);
    unsigned int fragmentShader = compileShader(readShader("shader.frag"), GL_FRAGMENT_SHADER);
    unsigned int shaderProgram = createShaderProgram(vertexShader, fragmentShader);

    mesh quad = {0};
    quad = createMesh(vertices, 12, indices, 6);

    entity quad_e = createEntity(&quad);
    setPosition(&quad_e, 0, 0, 0);
    setRotation(&quad_e, 0, 0, 0);

    entity quad_e2 = createEntity(&quad);
    setPosition(&quad_e2, 0, 2, 0);
    setRotation(&quad_e2, 0, 0, 0);

    glUseProgram(shaderProgram);

    vec3 camPos = {0,0,3};
    MVP mvp = createMVP(camPos, glm_rad(FOV), 0.01f, 100.0f);

    SDL_ShowWindow(state.window);

    state.isRunning = true;
    SDL_Event event;

    while (state.isRunning == true) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) state.isRunning = false;
        }
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        quad_e.transform.rotation[1] += glm_rad(1.0f);

        submitMVP(&mvp, shaderProgram);
        drawEntity(&quad_e, shaderProgram);
        drawEntity(&quad_e2, shaderProgram);

        SDL_GL_SwapWindow(state.window);
    }

    cleanup(&state);
}