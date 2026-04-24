#pragma once
#include <glad/glad.h>
#include "core/mesh.hpp"
#include "core/postprocess_pass.hpp"

class Renderer {
public:
    void init(int width, int height);
    void beginScene();
    void endScene();
    void addPass(const PostProcessPass& pass);
    void clearPasses();
    void setPassUniform(size_t index, const std::string& name, const UniformValue& value);
    void setDimensions(int width, int height);
    int getWidth() { return this->width; }
    int getHeight() { return this->height; } // ← was returning width, bug fixed
    ~Renderer();
private:
    unsigned int FBO;
    unsigned int depthTex;
    unsigned int colorTex;
    unsigned int normalTex;
    // ping-pong buffers
    unsigned int pingFBO, pongFBO;
    unsigned int pingTex, pongTex;
    // bloom bright-pass buffer
    unsigned int brightFBO, brightTex;
    Shader* blitShader;
    std::vector<PostProcessPass> passes;
    int width, height;
    Mesh* fullscreenQuadMesh;
    void initFullscreenQuad();
    void renderFullscreenQuad();
};