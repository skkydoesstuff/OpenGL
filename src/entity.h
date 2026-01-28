#pragma once

#include "mesh.h"
#include "transform.h"

typedef struct entity {
    mesh* mesh;
    transform transform;
} entity;

entity createEntity(mesh* meshRef);