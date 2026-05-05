#include "core/scene/scene.hpp"

void Scene::saveShaders(json& j) {
    for (const auto& [k, v] : this->rm->shaders.items()) {
        json shader;
        shader["name"] = k;
        shader["vert"] = v->vertName;
        shader["frag"] = v->fragName;

        j["shaders"].push_back(shader);
    }
}