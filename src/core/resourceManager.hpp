#pragma once

#include "core/resourceStore.hpp"
#include "core/renderer/mesh.hpp"
#include "core/renderer/shader.hpp"
#include "core/renderer/texture.hpp"
#include "core/renderer/material.hpp"
#include "core/renderer/postprocess_pass.hpp"

class ResourceManager {
public:
    ResourceStore<Mesh> meshes;
    ResourceStore<Shader> shaders;
    ResourceStore<Texture> textures;
    ResourceStore<Material> materials;
    ResourceStore<PostProcessPass> passes;
};