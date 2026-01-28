#include "entity.h"

entity createEntity(mesh* meshRef) {
    entity e;
    e.mesh = meshRef;
    e.transform = transform_identity();
    return e;
}

void setPosition(entity* e, float x, float y, float z) {
    e->transform.position[0] = x;
    e->transform.position[1] = y;
    e->transform.position[2] = z;
}

void setRotation(entity* e, float x, float y, float z) {
    e->transform.rotation[0] = x;
    e->transform.rotation[1] = y;
    e->transform.rotation[2] = z;
}

void setScale(entity* e, float x, float y, float z) {
    e->transform.scale[0] = x;
    e->transform.scale[1] = y;
    e->transform.scale[2] = z;
}