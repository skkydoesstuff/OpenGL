#include "cleanup.h"
#include <SDL3/SDL.h>

void cleanup(engineState* state) {
    SDL_GL_DestroyContext(state->context);
    SDL_DestroyWindow(state->window);

    SDL_Quit();
}