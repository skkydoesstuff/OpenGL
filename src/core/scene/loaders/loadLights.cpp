#include "core/scene/scene.hpp"

void Scene::loadLights(const json& j) {
    auto toVec3 = [](const json& a) {
        return glm::vec3(a[0], a[1], a[2]);
    };

    for (const auto& light : j["lights"]) {
        std::string name = light.value("name", "");

        Light* l = this->createLight(name);

        auto& position = light["position"];
        auto& ambient = light["ambient"];
        auto& diffuse = light["diffuse"];
        auto& specular = light["specular"];
        auto& constant = light["constant"];
        auto& linear = light["linear"];
        auto& quadratic = light["quadratic"];

        l->position = toVec3(position);
        l->ambient = toVec3(ambient);
        l->diffuse = toVec3(diffuse);
        l->specular = toVec3(specular);

        l->constant = constant;
        l->linear = linear;
        l->quadratic = quadratic;
    }
}