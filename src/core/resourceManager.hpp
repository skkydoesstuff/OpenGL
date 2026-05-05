#pragma once

#include "core/resourceStore.hpp"
#include "core/renderer/mesh.hpp"
#include "core/renderer/shader.hpp"
#include "core/renderer/texture.hpp"
#include "core/renderer/material.hpp"
#include "core/renderer/renderer.hpp"
#include "core/renderer/postprocess_pass.hpp"
#include "core/scene/objects/model.hpp"
#include "core/scene/objects/camera.hpp"
#include "core/scene/objects/light.hpp"

class ResourceManager {
public:
    ResourceStore<Mesh> meshes;
    ResourceStore<Shader> shaders;
    ResourceStore<Texture> textures;
    ResourceStore<Material> materials;
    UniqueResourceStore<PostProcessPass> passes;
    UniqueResourceStore<Model> models;
    UniqueResourceStore<Light> lights;
    UniqueResourceStore<Renderer> renderers;
    UniqueResourceStore<Camera> cameras;
};