#include "core/scene/scene.hpp"

void Scene::loadMeshes(const json& j) {
    for (const auto& mesh : j["meshes"]) {
        std::string name = mesh.value("name", "");
        std::string source = mesh.value("source", "");
        std::string material = mesh.value("base_material", "");

        
        std::shared_ptr<Mesh> m = this->createMesh(name, {}, {}, source);
        if (!material.empty()) {
            m->setMaterial(this->getMaterial(material));
        }
    }
}