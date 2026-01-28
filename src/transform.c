#include <cglm/cglm.h>
#include "transform.h"

transform transform_identity(void) {
    transform t = {0};
    glm_vec3_zero(t.position);
    glm_vec3_zero(t.rotation);
    glm_vec3_zero(t.scale);
    return t;
}

void transform_build_model(mat4 out, transform* t) {
    glm_mat4_identity(out);

    glm_translate(out, t->position);
    glm_rotate_x(out, t->rotation[0], out);
    glm_rotate_y(out, t->rotation[1], out);
    glm_rotate_z(out, t->rotation[2], out);
    glm_scale(out, (vec3){t->scale[0], t->scale[1], t->scale[2]});
}