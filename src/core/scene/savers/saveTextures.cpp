#include "core/scene/scene.hpp"
#include "nlohmann/json.hpp"

void Scene::saveTextures(json& j) {
    for (const auto& [k, v] : this->rm->textures.items()) {
        json texture;
        texture["name"] = k;
        texture["source"] = v->texSource;

        j["textures"].push_back(texture);
    }
}