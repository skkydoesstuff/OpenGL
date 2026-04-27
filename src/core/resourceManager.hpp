#pragma once

#include "core/resourceStore.hpp"
#include "core/mesh.hpp"
#include "core/shader.hpp"
#include "core/texture.hpp"
#include "core/material.hpp"
#include "core/postprocess_pass.hpp"

class ResourceManager {
public:
    ResourceStore<Mesh> meshes;
    ResourceStore<Shader> shaders;
    ResourceStore<Texture> textures;
    ResourceStore<Material> materials;
    ResourceStore<PostProcessPass> passes;
};