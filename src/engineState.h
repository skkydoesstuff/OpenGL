#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>

typedef struct engineState {
    SDL_Window* window;
    SDL_GLContext context;
    bool isRunning;
} engineState;