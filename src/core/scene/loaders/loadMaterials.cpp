#include "core/scene/scene.hpp"

void Scene::loadMaterials(const json& j) {
    for (const auto& material : j["materials"]) {
        std::string name = material.value("name", "");
        float shininess = material.value("shininess", 32.0f);
        std::string diffuse = material.value("diffuse", "");
        std::string specular = material.value("specular", "");
        std::string opacity = material.value("opacity", "");
        std::string normal = material.value("normal", "");

        std::shared_ptr<Material> mat = this->rm->materials.create(name);
        mat->shininess = shininess;

        std::shared_ptr<Texture> diff = this->rm->textures.get(diffuse);
        mat->diffuse = diff;

        if (!specular.empty()) {
            std::shared_ptr<Texture> spec = this->rm->textures.get(specular);
            mat->specular = spec;
        }

        if (!opacity.empty()) {
            std::shared_ptr<Texture> opac = this->rm->textures.get(opacity);
            mat->opacityMap = opac;
        }

        if (!normal.empty()) {
            std::shared_ptr<Texture> norm = this->rm->textures.get(normal);
            mat->normal = norm;
        }
    }
}