#pragma once

#include "mesh.h"
#include "transform.h"

typedef struct entity {
    mesh* mesh;
    transform transform;
} entity;

entity createEntity(mesh* meshRef);

void setPosition(entity* e, float x, float y, float z);
void setRotation(entity* e, float x, float y, float z);
void setScale(entity* e, float x, float y, float z);