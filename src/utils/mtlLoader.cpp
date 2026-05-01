#include "utils/mtlLoader.hpp"
#include "utils/fileUtils.hpp"
#include <sstream>
#include <fstream>
#include <iostream>

std::unordered_map<std::string, Material*>
MTLLoader::load(const std::string& path) {

    static std::string exeDir = getExecutableDirectory();
    static std::string assetDir = exeDir + "\\assets\\";

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
            std::string tex;
            iss >> tex;

            std::string fullPath = assetDir + tex;

            materials[current]->diffuse =
                std::make_shared<Texture>(fullPath);
        }

        else if (type == "map_Ks") {
            std::string tex;
            iss >> tex;

            std::string fullPath = assetDir + tex;

            materials[current]->specular =
                std::make_shared<Texture>(fullPath);
        }

        else if (type == "map_d") {
            std::string tex;
            iss >> tex;
            std::string fullPath = assetDir + tex;
            materials[current]->opacityMap = std::make_shared<Texture>(fullPath);
            // don't set hasCutoutMap here, decide later
            materials[current]->hasOpacityMap = true;
        }

        else if (type == "map_Bump" || type == "bump") {
            std::string token;
            std::string tex;
            while (iss >> token) {
                if (token[0] == '-') {
                    iss >> token; // skip the flag's value
                } else {
                    tex = token; // first non-flag token is the filename
                    break;
                }
            }
            if (!tex.empty()) {
                std::string fullPath = assetDir + tex;
                materials[current]->normal = std::make_shared<Texture>(fullPath);
                materials[current]->hasNormalMap = true;
            }
        }

        else if (type == "Ns") {
            float ns;
            iss >> ns;
            materials[current]->shininess = ns;
        }

        else if (type == "d") {
            float d;
            iss >> d;
            materials[current]->opacityValue = d;
            materials[current]->isTransparent = true;
        }

        else if (type == "Tr") {
            float tr;
            iss >> tr;
            materials[current]->opacityValue = 1.0f - tr;
        }
    }

    return materials;
}