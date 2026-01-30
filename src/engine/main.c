#include <GLAD/glad.h>
#include <SDL3/SDL.h>

#include <stdio.h>
#include <stdlib.h>

#include "game.h"

#define TITLE "window"
#define WIDTH 800
#define HEIGHT 600
#define FLAGS (SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_OPENGL)

typedef struct engineState {
    SDL_Window* window;
    SDL_GLContext context;
    bool isRunning;
} engineState;

void init(engineState* state) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "failed to initialize SDL: %s\n", SDL_GetError());
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    state->window = SDL_CreateWindow(TITLE, WIDTH, HEIGHT, FLAGS);
    if (!state->window) {
        fprintf(stderr, "failed to create window: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    state->context = SDL_GL_CreateContext(state->window);
    if (!state->context) {
        fprintf(stderr, "failed to create OpenGL context: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_GL_MakeCurrent(state->window, state->context);
    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        fprintf(stderr, "failed to load GLAD");
        exit(EXIT_FAILURE);
    }

    glViewport(0, 0, WIDTH, HEIGHT);
}

int main() {
    engineState state = {0};
    init(&state);

    setup();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    SDL_Event e;
    state.isRunning = true;
    while (state.isRunning == true) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) state.isRunning = false;
        }

        physicsLoop();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderLoop();

        logicLoop();

        SDL_GL_SwapWindow(state.window);
        SDL_Delay(1);
    }

    SDL_GL_DestroyContext(state.context);
    SDL_DestroyWindow(state.window);
    exit(EXIT_SUCCESS);
}