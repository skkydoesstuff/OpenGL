#include "core/scene/scene.hpp"

void Scene::loadTextures(const json& j) {
    for (const auto& texture : j["textures"]) {
        std::string name = texture.value("name", "");
        std::string source = texture.value("source", "");

        this->createTexture(name, source);
    }
}