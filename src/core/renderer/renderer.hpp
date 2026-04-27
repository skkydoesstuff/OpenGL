#pragma once
#include <glad/glad.h>
#include "core/renderer/mesh.hpp"
#include "core/renderer/postprocess_pass.hpp"
#include "core/renderer/renderTarget.hpp"

class Renderer {
public:
    struct AttachmentDesc {
        int        count;
        TextureDesc format = {GL_RGB16F, GL_RGB, GL_FLOAT};
    };
    struct RTDesc {
        AttachmentDesc color;
        TextureDesc    depth;
        bool           hasDepth = false;
        bool           msaa     = false;
    };

    void registerRT(const std::string& name, RTDesc desc);
    RenderTarget& getRT(const std::string& name);

    void bindTex(int slot, GLuint tex);

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

    std::unordered_map<std::string, RTDesc>        rtDescs;
    std::unordered_map<std::string, RenderTarget>  rts;

    std::vector<std::shared_ptr<PostProcessPass>> passes;

    int width, height;

    Mesh* fullscreenQuadMesh;
    void initFullscreenQuad();
    void renderFullscreenQuad();
};