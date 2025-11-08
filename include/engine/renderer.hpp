#pragma once 

#include <unordered_map>
#include <memory>
#include <string>

#include "engine/drawable.hpp"

class Camera;
class Shader;

class Renderer {
private:
    std::unordered_map<std::string, std::unique_ptr<Drawable>> drawables;

public:
    void add(const std::string& tag, std::unique_ptr<Drawable> d);
    Drawable* getDrawable(const std::string& tag);
    void drawAll(const Camera& camera, Shader& shader) const;
};