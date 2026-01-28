#pragma once
#include "cglm/cglm.h"

typedef struct transform {
    vec3 position;
    vec3 rotation;
    vec3 scale;
} transform;

transform transform_identity(void);
void transform_build_model(mat4 out, const transform* t);