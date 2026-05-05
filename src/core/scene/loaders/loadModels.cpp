#include "core/scene/scene.hpp"

void Scene::loadModels(const json& j) {
    auto toVec3 = [](const json& a) {
        return glm::vec3(a[0], a[1], a[2]);
    };

    for (const auto& model : j["models"]) {
        std::string name = model.value("name", "");
        std::string mesh = model.value("mesh", "");

        Model* m = this->createModel(name, mesh);

        auto& transform = model["transform"];

        m->transform.position = toVec3(transform["position"]);

        m->transform.rotation = toVec3(transform["rotation"]);

        m->transform.scale = toVec3(transform["scale"]);
    }
}