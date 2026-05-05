#include "core/scene/scene.hpp"

void Scene::saveLights(json& j) {
    for (auto& [k, v] : this->rm->lights.items()) {
        json light;

        light["name"] = k;

        light["position"] = {v->position.x, v->position.y, v->position.z};
        light["ambient"] = {v->ambient.x, v->ambient.y, v->ambient.z};
        light["diffuse"] = {v->diffuse.x, v->diffuse.y, v->diffuse.z};
        light["specular"] = {v->specular.x, v->specular.y, v->specular.z};

        light["constant"] = v->constant;
        light["linear"] = v->linear;
        light["quadratic"] = v->quadratic;

        j["lights"].push_back(light);
    }
}