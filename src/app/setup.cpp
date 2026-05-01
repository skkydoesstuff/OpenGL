#include "app/app.hpp"

#include "utils/fileUtils.hpp"
#include "utils/jsonHelpers.hpp"

void App::setup() {
    std::string exeDir = getExecutableDirectory();
    std::string assetDir = exeDir + "\\assets\\";
    
    scene->loadSceneFromJSON(assetDir + "scenes\\" + "test.json");

    auto cam = scene->createCamera(45.0f, (float)this->width/(float)this->height, 0.1f, 100.0f);
    cam->position = {0.0f, 0.0f, 1.0f};
    cam->rotation = {0.0f, -90.0f, 0.0f};

    this->settings = {
        .scanlineOn = false,
        .pixelatedOn = false,

        .scanThickness = 2.0f,
        .scanDarkness = 0.6f,
        .phosphorStrength = 1.5f,
        .glowStrength = 0.3f,
        .vignetteStrength = 0.35f,
        .brightBoost = 1.15f,

        .pixelSize = 4.0f
    };

    //scene->saveSceneToJSON(assetDir + "scenes\\" + "test.json");
}
 