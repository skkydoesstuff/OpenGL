#include "core/scene/scene.hpp"
#include "utils/objLoader.hpp"
#include "utils/fileUtils.hpp"

void Scene::saveMeshes(json& j) {
    std::string exeDir = getExecutableDirectory();
    std::string assetDir = exeDir + "\\assets\\";

    for (const auto& [k, v] : this->rm->meshes.items()) {
        json mesh;
        mesh["name"] = k;
        if (v->sourceFile.empty()) {
            std::string objPath = assetDir + k + ".obj";
            std::string mtlPath = assetDir + k + ".mtl";

            saveOBJ(objPath, mtlPath, v);
            v->sourceFile = objPath;
        }

        mesh["source"] = v->sourceFile;

        j["meshes"].push_back(mesh);
    }
}