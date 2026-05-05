#include "core/scene/scene.hpp"

void Scene::loadShaders(const json& j) {
    for (const auto& shader : j["shaders"]) {
        std::string name = shader.value("name", "");
        std::string vert = shader.value("vert", "");
        std::string frag = shader.value("frag", "");

        this->createShader(name, vert, frag);
    }
}