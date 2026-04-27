#include "app/app.hpp"

#include "core/camera.hpp"
#include "core/model.hpp"

#include "utils/fileUtils.hpp"
#include "utils/stb_image.h"

void App::setup() {
    std::string exeDir = getExecutableDirectory();
    std::string assetDir = exeDir + "\\assets\\";
    
    std::vector<float> vs = {
        // positions          // normals           // texcoords
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
        // Left face
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        // Right face
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    };

    scene.createMesh("cube", vs);

    /*                                 
    ----------- SHADER SETUP -----------
    */                                 
    scene.createShader("base", "base.vert", "base.frag");
    scene.createShader("scanline", "blit.vert", "scanline.frag");
    scene.createShader("outline", "blit.vert", "outline.frag");
    scene.createShader("pixelated", "blit.vert", "pixelated.frag");

    /*                                 
    ----------- TEXTURE SETUP -----------
    */          
    scene.createTexture("container", assetDir + "textures\\container.png");
    scene.createTexture("container_spec", assetDir + "textures\\container.spec.png");

    /*                                 
    ----------- MATERIAL SETUP -----------
    */     
    scene.createMaterial("container", 128.0f, "container", "container_spec");
    
    /*                                 
    ----------- MODEL SETUP -----------
    */     
    auto c = scene.createModel("cube", "base", "cube");
    c->transform.rotation = glm::vec3(.0f);
    c->transform.position = glm::vec3(0, 0, -3);
    c->setMaterial(scene.getMaterial("container"));

    auto l = scene.createLight("light");
    l->position  = glm::vec3(0.0f, 1.0f, -3.0f);
    l->ambient  = glm::vec3(0.15f);
    l->diffuse  = glm::vec3(3.0f);
    l->specular  = glm::vec3(1.0f);
    l->constant  = 1.0f;
    l->linear    = 0.0;
    l->quadratic = 0.0;

    auto cam = scene.createCamera("main", 45.0f, (float)this->width/(float)this->height, 0.1f, 100.0f);
    cam->position = {0.0f, 0.0f, 1.0f};
    cam->rotation = {0.0f, -90.0f, 0.0f};

    this->settings = {};

    this->settings.scanlineOn = false;
    this->settings.outlineOn = false;
    this->settings.pixelatedOn = false;

    this->settings.normThresh = 0.2f;
    this->settings.edgeStrength = 1.5f;
    this->settings.depthThresh = 0.01f;
    this->settings.edgeWidth = 2.0f;

    this->settings.scanThickness = 2.0f;
    this->settings.scanDarkness = 0.6f;
    this->settings.phosphorStrength = 1.5f;
    this->settings.glowStrength = 0.3f;
    this->settings.vignetteStrength = 0.35f;
    this->settings.brightBoost = 1.15f;
    
    this->settings.pixelSize = 4.0f;

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
        "outline",
        scene.getShader("outline"),
        {
            {"uNormalThreshold", this->settings.normThresh},
            {"uEdgeStrength", this->settings.edgeStrength},
            {"uDepthThreshold", this->settings.depthThresh},
            {"uEdgeWidth", this->settings.edgeWidth},
        }
    );

    scene.createPass(
        "pixelated",
        scene.getShader("pixelated"),
        {{"uPixelSize", this->settings.pixelSize}}
    );

    auto r = scene.createRenderer("main");
    r->init(this->width, this->height);
}
 