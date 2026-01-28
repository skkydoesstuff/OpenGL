#include "entity.h"

entity createEntity(mesh* meshRef) {
    entity e;
    e.mesh = meshRef;
    e.transform = transform_identity();
    return e;
}