#include "core/scene/scene.hpp"

void Scene::savePasses(json& j) {
    for (const auto& [k, v] : this->rm->passes.items()) {
        json pass;
        pass["name"] = k;
        pass["shader"] = v->shaderName;

        json uniforms;
        for (const auto& [key, val] : v->uniforms) {
            std::visit([&](auto&& v) {
                using T = std::decay_t<decltype(v)>;
                if constexpr (std::is_same_v<T, int>)            uniforms[key] = v;
                else if constexpr (std::is_same_v<T, float>)     uniforms[key] = v;
                else if constexpr (std::is_same_v<T, glm::vec2>) uniforms[key] = { v.x, v.y };
                else if constexpr (std::is_same_v<T, glm::vec3>) uniforms[key] = { v.x, v.y, v.z };
                else if constexpr (std::is_same_v<T, glm::vec4>) uniforms[key] = { v.x, v.y, v.z, v.w };
                else if constexpr (std::is_same_v<T, glm::mat4>) {
                    json mat = json::array();
                    for (int i = 0; i < 4; i++)
                        for (int j = 0; j < 4; j++)
                            mat.push_back(v[i][j]);
                    uniforms[key] = mat;
                }
            }, val);
        }
        pass["uniforms"] = uniforms;
        j["passes"].push_back(pass);
    }
}