#include <GLAD/glad.h>
#include <SDL3/SDL.h>

#include "engineState.h"
#include "error_handling.h"
#include "init.h"
#include "defines.h"

void initContext(const char * title, unsigned int w, unsigned int h, SDL_WindowFlags windowFlags, engineState* state) {
    if (!SDL_Init(INIT)) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
    }
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    state->window = SDL_CreateWindow(title, w, h, windowFlags);
    if (!state->window) {
        throw_error("Failed to create window!");
    }

    state->context = SDL_GL_CreateContext(state->window);
    if (!state->context) {
        throw_error("Failed to create OpenGL Context!");
    }

    SDL_GL_MakeCurrent(state->window, state->context);
    gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress);

    glEnable(GL_DEPTH_TEST);
    if (WIREFRAME == true) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
}