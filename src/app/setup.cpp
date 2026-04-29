#include "app/app.hpp"

#include "core/scene/camera.hpp"
#include "core/scene/model.hpp"

#include "utils/fileUtils.hpp"
#include "utils/stb_image.h"

void App::setup() {
    std::string exeDir = getExecutableDirectory();
    std::string assetDir = exeDir + "\\assets\\";
    
    scene.createMesh("tree", {}, {}, "seafront");

    /*                                 
    ----------- SHADER SETUP -----------
    */                                 
    scene.createShader("base", "base.vert", "base.frag");
    scene.createShader("scanline", "blit.vert", "scanline.frag");
    scene.createShader("outline", "blit.vert", "outline.frag");
    scene.createShader("pixelated", "blit.vert", "pixelated.frag");

    /*                                 
    ----------- MODEL SETUP -----------
    */     
    auto c = scene.createModel("tree", "tree");
    c->transform.rotation = glm::vec3(.0f);
    c->transform.position = glm::vec3(0, 0, 0);
    c->transform.scale = glm::vec3(0.50f);

    auto l = scene.createLight("light");
    l->position  = glm::vec3(1.7f, 2.3f, -2.9f);
    l->ambient  = glm::vec3(0.15f, 0.15f, 0.0f);
    l->diffuse  = glm::vec3(2.0f, 2.0f, 2.0f);
    l->specular  = glm::vec3(0.5f, 0.5f, 0.5f);
    l->constant  = 1.0f;
    l->linear    = 0.0;
    l->quadratic = 0.0;

    auto cam = scene.createCamera(45.0f, (float)this->width/(float)this->height, 0.1f, 100.0f);
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

    scene.createPass(
        "scanline",
        scene.getShader("scanline"),
        {
            {"uScanlineThickness", this->settings.scanThickness},
            {"uScanlineDarkness", this->settings.scanDarkness},
            {"uPhosphorStrength", this->settings.phosphorStrength},
            {"uGlowStrength", this->settings.glowStrength},
            {"uVignetteStrength", this->settings.vignetteStrength},
            {"uBrightBoost", this->settings.brightBoost},
        }
    );

    scene.createPass(
        "pixelated",
        scene.getShader("pixelated"),
        {{"uPixelSize", this->settings.pixelSize}}
    );

    auto r = scene.createRenderer();
    r->init(this->width, this->height);
}
 