#include "utils/mtlLoader.hpp"
#include "utils/fileUtils.hpp"
#include "core/renderer/texture.hpp"
#include <sstream>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <algorithm>

void saveMTL(const std::string& path, std::unordered_map<std::string, Material*> materials) {
    std::ostringstream mtl;
    std::filesystem::path mtlDir = std::filesystem::path(path).parent_path();

    auto relPath = [&](const std::shared_ptr<Texture>& tex) -> std::string {
        auto rel = std::filesystem::relative(tex->texSource, mtlDir);
        std::string s = rel.string();
        std::replace(s.begin(), s.end(), '\\', '/');
        return s;
    };

    for (auto& [k, mat] : materials) {
        mtl << "newmtl " << k << '\n';
        mtl << "Ns " << mat->shininess << '\n';
        mtl << "d " << mat->opacityValue << '\n';
        if (mat->diffuse)
            mtl << "map_Kd " << relPath(mat->diffuse) << '\n';
        if (mat->specular)
            mtl << "map_Ks " << relPath(mat->specular) << '\n';
        if (mat->opacityMap)
            mtl << "map_d " << relPath(mat->opacityMap) << '\n';
        if (mat->normal)
            mtl << "map_Bump " << relPath(mat->normal) << '\n';
        mtl << '\n';
    }

    std::filesystem::create_directories(mtlDir);
    std::ofstream out(path);
    if (!out.is_open()) {
        std::cerr << "Failed to write MTL: " << path << "\n";
        return;
    }
    out << mtl.str();
    std::cout << "Saved MTL: " << path << "\n";
}

std::unordered_map<std::string, Material*>
MTLLoader::load(const std::string& path) {
    std::filesystem::path mtlDir = std::filesystem::path(path).parent_path();

    auto resolveTex = [&](const std::string& tex) -> std::string {
        return std::filesystem::weakly_canonical(mtlDir / tex).string();
    };

    std::unordered_map<std::string, Material*> materials;
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open MTL: " << path << "\n";
        return materials;
    }

    std::string line;
    std::string current;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "newmtl") {
            iss >> current;
            materials[current] = new Material();
        }
        else if (type == "map_Kd") {
            std::string tex; iss >> tex;
            std::string fullPath = resolveTex(tex);
            if (!std::filesystem::exists(fullPath)) {
                std::cerr << "Texture not found, skipping: " << fullPath << "\n";
            } else {
                materials[current]->diffuse = std::make_shared<Texture>(fullPath);
            }
        }
        else if (type == "map_Ks") {
            std::string tex; iss >> tex;
            std::string fullPath = resolveTex(tex);
            if (!std::filesystem::exists(fullPath)) {
                std::cerr << "Texture not found, skipping: " << fullPath << "\n";
            } else {
                materials[current]->specular = std::make_shared<Texture>(fullPath);
            }
        }
        else if (type == "map_d") {
            std::string tex; iss >> tex;
            std::string fullPath = resolveTex(tex);
            if (!std::filesystem::exists(fullPath)) {
                std::cerr << "Texture not found, skipping: " << fullPath << "\n";
            } else {
                materials[current]->opacityMap = std::make_shared<Texture>(fullPath);
                materials[current]->hasOpacityMap = true;
            }
        }
        else if (type == "map_Bump" || type == "bump") {
            std::string token;
            std::string tex;
            while (iss >> token) {
                if (token[0] == '-') {
                    iss >> token; // skip flag value
                } else {
                    tex = token;
                    break;
                }
            }
            if (!tex.empty()) {
                std::string fullPath = resolveTex(tex);
                if (!std::filesystem::exists(fullPath)) {
                    std::cerr << "Texture not found, skipping: " << fullPath << "\n";
                } else {
                    materials[current]->normal = std::make_shared<Texture>(fullPath);
                    materials[current]->hasNormalMap = true;
                }
            }
        }
        else if (type == "Ns") {
            float ns; iss >> ns;
            materials[current]->shininess = ns;
        }
        else if (type == "d") {
            float d; iss >> d;
            materials[current]->opacityValue = d;
            materials[current]->isTransparent = (d < 1.0f);
        }
        else if (type == "Tr") {
            float tr; iss >> tr;
            materials[current]->opacityValue = 1.0f - tr;
            materials[current]->isTransparent = (tr > 0.0f);
        }
    }
    return materials;
}