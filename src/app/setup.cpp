#include "app/app.hpp"

#include "core/mesh.hpp"
#include "core/model.hpp"
#include "core/postprocess_pass.hpp"
#include "core/shader.hpp"

#include "utils/fileUtils.hpp"
#include "utils/stb_image.h"

#include <memory>

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

    auto makeShader = [&](const std::string& name,
                          const std::string& vert,
                          const std::string& frag) {
        return this->rm.shaders.create(name, vert, frag);
    };

    auto makeMesh = [&](const std::string& name,
                        const std::vector<float>& vertices,
                        const std::vector<unsigned int> indices = {}) {
        std::shared_ptr<Mesh> m = this->rm.meshes.create(name, vertices, 8, indices);
        m->addVertexAttribute(0, 3, GL_FLOAT, 8 * sizeof(float), (const void*)0);
        m->addVertexAttribute(1, 3, GL_FLOAT, 8 * sizeof(float), (const void*)(sizeof(float) * 3));
        m->addVertexAttribute(2, 2, GL_FLOAT, 8 * sizeof(float), (const void*)(sizeof(float) * 6));

        return m;
    };

    auto makePass = [&](const std::string& name,
                        std::shared_ptr<Shader> shader,
                        std::unordered_map<std::string, UniformValue> uniforms = {},
                        std::unordered_map<std::string, std::string> extraTextures = {},
                        std::string output = "") {
        std::shared_ptr<PostProcessPass> pass = this->rm.passes.create(name);
        pass->shader = shader;
        pass->uniforms = std::move(uniforms);
        pass->extraTextures = std::move(extraTextures);
        pass->saveOutputAs = std::move(output);

        return pass;
    };

    std::shared_ptr<Mesh> m = makeMesh("cube", vs);

    std::shared_ptr<Shader> baseShader = makeShader("base", "base.vert", "base.frag");
    std::shared_ptr<Shader> scanlineShader = makeShader("scanline", "blit.vert", "scanline.frag");
    std::shared_ptr<Shader> outlineShader = makeShader("outline", "blit.vert", "outline.frag");
    std::shared_ptr<Shader> pixelatedShader = makeShader("pixelated", "blit.vert", "pixelated.frag");

    std::shared_ptr<Shader> thresholdShader = makeShader("threshold", "blit.vert", "threshold.frag");
    std::shared_ptr<Shader> blurShader = makeShader("blur", "blit.vert", "blur.frag");
    std::shared_ptr<Shader> compositeShader = makeShader("composite", "blit.vert", "composite.frag");

    std::shared_ptr<Texture> tex = this->rm.textures.create("container", assetDir + "textures\\pubert.png");
    std::shared_ptr<Texture> texSpec = this->rm.textures.create("container_spec", assetDir + "textures\\pubert.spec.png");

    std::shared_ptr<Material> mat = this->rm.materials.create("container");
    mat->diffuse = tex;
    mat->specular = texSpec;
    mat->shininess = 32.0f;

    std::shared_ptr<Model> c = this->rm.models.create("cube", baseShader, m);
    c->transform.rotation = glm::vec3(.0f);
    c->transform.position = glm::vec3(0, 0, -3);

    std::shared_ptr<Light> l = this->rm.lights.create("light");
    l->position  = glm::vec3(0.0f, 1.0f, -3.0f);  // closer, less height
    l->ambient  = glm::vec3(0.15f);  // nearly zero
    l->diffuse  = glm::vec3(3.0f);
    l->specular  = glm::vec3(1.0f);  // slight red tint on highlights
    l->constant  = 1.0f;
    l->linear    = 0.0;
    l->quadratic = 0.0;

    this->cam = new Camera(45.0f, (float)this->width/(float)this->height, 0.1f, 100.0f);
    cam->position = {0.0f, 0.0f, 1.0f};
    cam->rotation = {0.0f, -90.0f, 0.0f};

    this->settings = {};
    

    this->settings.scanlineOn = false;
    this->settings.outlineOn = false;
    this->settings.pixelatedOn = false;
    this->settings.bloomOn = false;

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

    this->settings.thresholdVal = 0.8f;
    this->settings.intensity = 1.2f;
    this->settings.blurPasses = 1;

    std::shared_ptr<PostProcessPass> threshold = makePass(
        "threshold",
        thresholdShader,
        {{"uThreshold", this->settings.normThresh}},
        {},
        "bloomThreshold"
    );

    std::shared_ptr<PostProcessPass> blurH = makePass(
        "blurH",
        blurShader,
        {{"uHorizontal", 1}}
    );

    std::shared_ptr<PostProcessPass> blurV = makePass(
        "blurV",
        blurShader,
        {{"uHorizontal", 0}},
        {},
        "bloomBlurred"
    );

    std::shared_ptr<PostProcessPass> composite = makePass(
        "composite",
        compositeShader,
        {{"uIntensity", this->settings.intensity}},
        {
            {"uBloom", "bloomBlurred"},
            {"uScene", "scene"}
        }
    );

    std::shared_ptr<PostProcessPass> scanline = makePass(
        "scanline",
        scanlineShader,
        {
            {"uScanlineThickness", this->settings.scanThickness},
            {"uScanlineDarkness", this->settings.scanDarkness},
            {"uPhosphorStrength", this->settings.phosphorStrength},
            {"uGlowStrength", this->settings.glowStrength},
            {"uVignetteStrength", this->settings.vignetteStrength},
            {"uBrightBoost", this->settings.brightBoost},
        }
    );

    std::shared_ptr<PostProcessPass> outline = makePass(
        "outline",
        outlineShader,
        {
            {"uNormalThreshold", this->settings.normThresh},
            {"uEdgeStrength", this->settings.edgeStrength},
            {"uDepthThreshold", this->settings.depthThresh},
            {"uEdgeWidth", this->settings.edgeWidth},
        }
    );

    std::shared_ptr<PostProcessPass> pixelated = makePass(
        "pixelated",
        pixelatedShader,
        {{"uPixelSize", this->settings.pixelSize}}
    );

    std::shared_ptr<Renderer> r = this->rm.renderers.create("main");
    r->init(this->width, this->height);
}
 