#pragma once

#include <cglm/cglm.h>

typedef struct MVP {
    mat4 view;
    mat4 projection;
} MVP;

MVP createMVP(vec3 cameraPosition, float fovInRadians, float nearPlane, float farPlane);
void submitMVP(MVP* mvp, unsigned int shaderProgram);