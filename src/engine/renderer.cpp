#include "engine/renderer.hpp"
#include "engine/drawable.hpp"
#include "engine/shader.hpp"
#include "engine/camera.hpp"

#include <unordered_map>
#include <string>

void Renderer::add(const std::string& tag, std::unique_ptr<Drawable> d) {
    drawables[tag] = std::move(d);
}

Drawable* Renderer::getDrawable(const std::string& tag) {
    auto it = drawables.find(tag);
    if (it != drawables.end())
        return it->second.get();
    return nullptr;
}

void Renderer::drawAll(const Camera& camera, Shader& shader) const {
    for (const auto& pair : drawables) {
        pair.second->draw(camera, shader);
    }
}