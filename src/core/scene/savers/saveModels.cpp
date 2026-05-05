#include "core/scene/scene.hpp"

void Scene::saveModels(json& j) {
    for (const auto& [k, v] : this->rm->models.items()) {
        json model;

        model["name"] = k;
        model["mesh"] = v->meshSourceName;

        json transform;

        transform["position"] = {
            v->transform.position.x,
            v->transform.position.y,
            v->transform.position.z
        };

        transform["rotation"] = {
            v->transform.rotation.x,
            v->transform.rotation.y,
            v->transform.rotation.z
        };

        transform["scale"] = {
            v->transform.scale.x,
            v->transform.scale.y,
            v->transform.scale.z
        };

        model["transform"] = transform;

        j["models"].push_back(model);
    }
}