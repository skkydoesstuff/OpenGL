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

    std::shared_ptr<Mesh> m = this->rm.meshes.create("cube", vs, 8);
    m->addVertexAttribute(0, 3, GL_FLOAT, 8 * sizeof(float), (const void*)0);
    m->addVertexAttribute(1, 3, GL_FLOAT, 8 * sizeof(float), (const void*)(sizeof(float) * 3));
    m->addVertexAttribute(2, 2, GL_FLOAT, 8 * sizeof(float), (const void*)(sizeof(float) * 6));

    std::shared_ptr<Shader> baseShader = this->rm.shaders.create("base", "base.vert", "base.frag");
    std::shared_ptr<Shader> scanlineShader = this->rm.shaders.create("scanline", "blit.vert", "scanline.frag");
    std::shared_ptr<Shader> outlineShader = this->rm.shaders.create("outline", "blit.vert", "outline.frag");

    std::shared_ptr<Texture> tex = this->rm.textures.create("container", assetDir + "textures\\container.png");
    std::shared_ptr<Texture> texSpec = this->rm.textures.create("container_spec", assetDir + "textures\\container.spec.png");

    std::shared_ptr<Material> mat = this->rm.materials.create("container");
    mat->diffuse = tex;
    mat->specular = texSpec;
    mat->shininess = 32.0f;

    std::shared_ptr<Model> c = this->rm.models.create("cube", baseShader, m);
    c->transform.rotation = glm::vec3(.0f);
    c->transform.position = glm::vec3(0, 0, -3);

    std::shared_ptr<Light> l = this->rm.lights.create("light");
    l->position = glm::vec3(0, 2, -3);
    l->ambient = glm::vec3(0.5f);
    l->diffuse = glm::vec3(1.0f);
    l->specular = glm::vec3(1.0f);

    this->cam = new Camera(45.0f, (float)this->width/(float)this->height, 0.1f, 100.0f);
    cam->position = {0.0f, 0.0f, 1.0f};
    cam->rotation = {0.0f, -90.0f, 0.0f};
    
    PostProcessPass outline;
    outline.shader = outlineShader;
    outline.uniforms["uNormalThreshold"] = 0.20f;
    outline.uniforms["uEdgeStrength"] = 1.5f;
    outline.uniforms["uDepthThreshold"] = 0.01f;
    outline.uniforms["uEdgeWidth"] = 2.0f;

    PostProcessPass scanline;
    scanline.shader = scanlineShader;
    scanline.uniforms["DARKNESS"] = 1.0f;
    scanline.uniforms["THICKNESS"] = 2.0f;
    scanline.uniforms["BRIGHTBOOST"] = 1.1f;

    std::shared_ptr<Renderer> r = this->rm.renderers.create("main");
    r->init(this->width, this->height);
    r->addPass(scanline);
    r->addPass(outline);
}
 