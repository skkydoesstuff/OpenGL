#pragma once
#include <glad/glad.h>
#include "core/mesh.hpp"
#include "core/renderer/postprocess_pass.hpp"
#include "core/renderer/renderTarget.hpp"

class Renderer {
public:
    void init(int width, int height);
    void beginScene();
    void endScene();
    void addPass(std::shared_ptr<PostProcessPass> pass);
    void clearPasses();
    void setPassUniform(size_t index, const std::string& name, const UniformValue& value);
    void setDimensions(int width, int height);
    int getWidth() { return this->width; }
    int getHeight() { return this->height; } // ← was returning width, bug fixed
    ~Renderer();
private:
    static constexpr int msaaSamples = 4; // 2, 4, or 8

    Shader* blitShader;

    RenderTarget msaa;
    RenderTarget scene;
    RenderTarget ping;
    RenderTarget pong;

    std::vector<std::shared_ptr<PostProcessPass>> passes;

    int width, height;

    Mesh* fullscreenQuadMesh;
    void initFullscreenQuad();
    void renderFullscreenQuad();
};