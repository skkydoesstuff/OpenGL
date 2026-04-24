#pragma once

#include "core/resourceStore.hpp"
#include "core/mesh.hpp"
#include "core/shader.hpp"
#include "core/model.hpp"
#include "core/light.hpp"
#include "core/texture.hpp"
#include "core/material.hpp"
#include "core/renderer.hpp"

class ResourceManager {
public:
    ResourceStore<Mesh> meshes;
    ResourceStore<Shader> shaders;
    ResourceStore<Model> models;
    ResourceStore<Light> lights;
    ResourceStore<Texture> textures;
    ResourceStore<Material> materials;
    ResourceStore<Renderer> renderers;
};