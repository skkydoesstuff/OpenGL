#pragma once
#include <glad/glad.h>
#include "core/mesh.hpp"
#include "core/postprocess_pass.hpp"

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

    unsigned int msaaFBO = 0;
    unsigned int msaaColorTex = 0;
    unsigned int msaaDepthTex = 0;
    unsigned int msaaNormalTex = 0;

    unsigned int FBO;
    unsigned int depthTex;
    unsigned int colorTex;
    unsigned int normalTex;
    // ping-pong buffers
    unsigned int pingFBO, pongFBO;
    unsigned int pingTex, pongTex;

    unsigned int bloomTex, bloomFBO;

    // bloom bright-pass buffer
    unsigned int brightFBO, brightTex;
    Shader* blitShader;

    std::vector<std::shared_ptr<PostProcessPass>> passes;

    int width, height;

    Mesh* fullscreenQuadMesh;
    void initFullscreenQuad();
    void renderFullscreenQuad();
};