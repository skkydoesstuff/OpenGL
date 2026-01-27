#define SDL_MAIN_HANDLED

#include <glad/glad.h>
#include <SDL3/SDL.h>

#include <stdio.h>
#include <stdbool.h>

#include "init.h"
#include "error_handling.h"
#include "engineState.h"
#include "cleanup.h"
#include "shaders.h"
#include "mesh.h"

#define WINDOW_TITLE "title"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_FLAGS (SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL)

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
    unsigned int program = createShaderProgram(vertexShader, fragmentShader);

    mesh quad = {0};
    quad = createMesh(vertices, 12, indices, 6);
    glBindVertexArray(quad.VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    glUseProgram(program);
    SDL_ShowWindow(state.window);

    state.isRunning = true;
    SDL_Event event;
    while (state.isRunning == true) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) state.isRunning = false;
        }
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        drawMesh(&quad);

        SDL_GL_SwapWindow(state.window);
    }

    cleanup(&state);
}