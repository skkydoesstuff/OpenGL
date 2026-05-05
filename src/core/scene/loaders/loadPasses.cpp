#include "core/scene/scene.hpp"

void Scene::loadPasses(const json& j) {
    for (const auto& pass : j["passes"]) {
        std::string name   = pass.value("name", "");
        std::string shader = pass.value("shader", "");

        std::unordered_map<std::string, UniformValue> uniforms;
        std::unordered_map<std::string, std::string> textures;

        // ---- uniforms ----
        const auto& u = pass["uniforms"];

        for (auto it = u.begin(); it != u.end(); ++it) {
            const auto& key = it.key();
            const auto& val = it.value();

            if (val.is_number_float()) {
                uniforms[key] = val.get<float>();
            }
            else if (val.is_number_integer()) {
                uniforms[key] = val.get<int>();
            }
            else if (val.is_array() && val.size() == 2) {
                uniforms[key] = glm::vec2(val[0], val[1]);
            }
            else if (val.is_array() && val.size() == 3) {
                uniforms[key] = glm::vec3(val[0], val[1], val[2]);
            }
            else if (val.is_array() && val.size() == 4) {
                uniforms[key] = glm::vec4(val[0], val[1], val[2], val[3]);
            }
            else if (val.is_array() && val.size() == 16) {
                glm::mat4 m(1.0f);
                for (int i = 0; i < 16; i++)
                    m[i / 4][i % 4] = val[i];
                uniforms[key] = m;
            }
        }

        // ---- textures ----
        if (pass.contains("textures")) {
            const auto& t = pass["textures"];

            for (auto it = t.begin(); it != t.end(); ++it) {
                textures[it.key()] = it.value().get<std::string>();
            }
        }

        this->createPass(name, shader, uniforms, textures, "");
    }
}