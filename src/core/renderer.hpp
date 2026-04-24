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

    ~Renderer();

private:
    unsigned int FBO;
    unsigned int depthTex;
    unsigned int colorTex;
    unsigned int normalTex;

    // ping-pong buffers
    unsigned int pingFBO, pongFBO;
    unsigned int pingTex, pongTex;

    Shader* blitShader;

    std::vector<PostProcessPass> passes;

    int width, height;

    Mesh* fullscreenQuadMesh;

    void initFullscreenQuad();
    void renderFullscreenQuad();
};