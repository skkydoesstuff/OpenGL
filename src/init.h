#pragma once

#include <GLAD/glad.h>
#include <SDL3/SDL.h>

#include "engineState.h"
#include "error_handling.h"

#define INIT (SDL_INIT_VIDEO)

void initContext(const char * title, unsigned int w, unsigned int h, SDL_WindowFlags windowFlags, engineState* state);