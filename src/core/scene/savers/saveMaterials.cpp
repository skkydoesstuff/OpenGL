#include "core/scene/scene.hpp"
#include "nlohmann/json.hpp"

void Scene::saveMaterials(json& j) {
    for (const auto& [k, v] : this->rm->materials.items()) {
        json material;

        material["name"] = k;
        material["shininess"] = std::to_string(v->shininess);

        if (v->diffuse)
            material["diffuse"] = v->diffuse->name;
        if (v->specular)
            material["specular"] = v->specular->name;
        if (v->opacityMap)
            material["opacity"] = v->opacityMap->name;
        if (v->normal)
            material["normal"] = v->normal->name;

        j["materials"].push_back(material);
    }
}