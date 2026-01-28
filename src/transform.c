#include <cglm/cglm.h>
#include "transform.h"

transform transform_identity(void) {
    transform t = {0};
    glm_vec3_zero(t.position);
    glm_vec3_zero(t.rotation);
    glm_vec3_copy((vec3){1.0f, 1.0f, 1.0f}, t.scale);
    return t;
}

void transform_build_model(mat4 out, const transform* t) {
    glm_mat4_identity(out);

    glm_translate(out, (vec3){t->position[0], t->position[1], t->position[2]});
    glm_rotate_x(out, t->rotation[0], out);
    glm_rotate_y(out, t->rotation[1], out);
    glm_rotate_z(out, t->rotation[2], out);
    glm_scale(out, (vec3){t->scale[0], t->scale[1], t->scale[2]});
}