#include "core/renderer/renderer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/renderer/shader.hpp"

#include "renderTarget.hpp"
#include "utils/fileUtils.hpp"
#include "utils/debugUtils.hpp"

#include "core/renderer/mesh.hpp"
#include "core/renderer/postprocess_pass.hpp"
#include "core/renderer/frameData.hpp"
#include "core/renderer/renderContext.hpp"

#include <iostream>
#include <algorithm>

Renderer::~Renderer() {}

void Renderer::initFullscreenQuad() {
    std::vector<float> quadVertices = {
        // positions   // texcoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    this->fullscreenQuadMesh = std::make_unique<Mesh>(quadVertices, 4);
    this->fullscreenQuadMesh->addVertexAttribute(0, 2, GL_FLOAT, 4 * sizeof(float), (void*)0);
    this->fullscreenQuadMesh->addVertexAttribute(1, 2, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void Renderer::renderFullscreenQuad() {
    this->fullscreenQuadMesh->draw();
}

void Renderer::addPass(PostProcessPass* pass) {
    this->passes.push_back(pass);
}

void Renderer::clearPasses() {
    this->passes.clear();
}

void Renderer::registerRT(const std::string& name, RTDesc desc) {
    rtDescs[name] = desc;
}

RenderTarget& Renderer::getRT(const std::string& name) {
    return rts.at(name);
}

void Renderer::init(int width, int height) {
    this->width  = width;
    this->height = height;

    TextureDesc hdr = {GL_RGBA16F, GL_RGBA, GL_FLOAT};
    TextureDesc depth = {GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT};

    this->registerRT("msaa", {.color = {2, hdr}, .depth = depth, .hasDepth = true, .msaa = true});
    this->registerRT("scene", {.color = {2, hdr}, .depth = depth, .hasDepth = true, .msaa = false});
    this->registerRT("ping", {.color = {2, hdr}, .depth = depth, .hasDepth = true, .msaa = false});
    this->registerRT("pong", {.color = {2, hdr}, .depth = depth, .hasDepth = true, .msaa = false});

    for (auto& [name, desc] : rtDescs) {
        auto& rt = rts[name];
        rt.init(width, height, desc.msaa ? msaaSamples : 0);
        for (int i = 0; i < desc.color.count; i++)
            rt.attachColor(GL_COLOR_ATTACHMENT0 + i, desc.color.format);
        if (desc.hasDepth)
            rt.attachDepth(desc.depth);
        rt.build();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    initFullscreenQuad();
    blitShader = std::make_unique<Shader>("blit.vert", "blit.frag");
}

void Renderer::resolveMSAA() {
    auto& msaa  = rts.at("msaa");
    auto& scene = rts.at("scene");

    glBindFramebuffer(GL_READ_FRAMEBUFFER, msaa.fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, scene.fbo);

    // Resolve attachment 0 (color)
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    // Resolve attachment 1 (normals)
    glReadBuffer(GL_COLOR_ATTACHMENT1);
    glDrawBuffer(GL_COLOR_ATTACHMENT1);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    // Restore draw buffers
    GLenum drawBufs[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, drawBufs);

    // Resolve depth
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

// extra textures start after these reserved slots
constexpr unsigned int kSlotColor = 0;
constexpr unsigned int kSlotDepth = 1;
constexpr unsigned int kSlotNormal = 2;

unsigned int Renderer::postProcessChain() {
    auto& scene = rts.at("scene");
    auto& ping  = rts.at("ping");

    std::unordered_map<std::string, GLuint> snapshots;
    snapshots["scene"] = scene.getColor(0);

    RenderTarget* input  = &scene;
    RenderTarget* output = &ping;

    for (auto& pass : passes) {
        output->bindForDraw();
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        pass->shader->bind();
        glBindTextureUnit(kSlotColor, input->getColor(0));
        pass->shader->setUniformInt("uTexture", kSlotColor);
        glBindTextureUnit(kSlotDepth, scene.getDepth());
        pass->shader->setUniformInt("uDepth", kSlotDepth);
        glBindTextureUnit(kSlotNormal, scene.getColor(1));
        pass->shader->setUniformInt("uNormal", kSlotNormal);
        pass->shader->setUniformVec2("uOutputSize", glm::vec2(width, height));
        pass->applyUniforms();

        unsigned int slot = 3;
        for (auto& [uniformName, snapshotName] : pass->extraTextures) {
            auto it = snapshots.find(snapshotName);
            if (it != snapshots.end()) {
                glBindTextureUnit(slot, it->second);
                pass->shader->setUniformInt(uniformName, slot);
                slot++;
            } else {
                DEBUG_PRINT(std::cout << "MISSING snapshot: " << snapshotName << "\n");
            }
        }

        renderFullscreenQuad();

        if (!pass->saveOutputAs.empty())
            snapshots[pass->saveOutputAs] = output->getColor(0);

        std::swap(input, output);
    }

    return input->getColor(0);
}

void Renderer::finalBlit(unsigned int finalTexture) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    blitShader->bind();
    glBindTextureUnit(0, finalTexture);
    blitShader->setUniformInt("uTexture", 0);
    renderFullscreenQuad();
}

void Renderer::beginScene() {
    auto& msaa = rts.at("msaa");
    msaa.bindForDraw();
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::endScene() {
    this->resolveMSAA();
    unsigned int final = this->postProcessChain();
    this->finalBlit(final);

    glEnable(GL_DEPTH_TEST);
}

void Renderer::setDimensions(int width, int height) {
    this->width  = width;
    this->height = height;
    for (auto& [name, rt] : rts)
        rt.resize(width, height);
    glBindTextureUnit(GL_TEXTURE_2D, 0);
}

void Renderer::renderScene(const FrameSnapshot& frameSnapshot) {
    beginScene();

    glBindTextureUnit(2, 0);

    auto& ctx = frameSnapshot.ctx;
    auto& cmdList = frameSnapshot.frame.commands;

    ctx.shader->bind();

    ctx.shader->setUniformMat4("view", ctx.view);
    ctx.shader->setUniformMat4("projection", ctx.projection);
    ctx.shader->setUniformVec3("viewPos", ctx.cameraPosition);

    // upload lights once
    int i = 0;
    for (auto& light : frameSnapshot.frame.lights) {
        light->upload(*ctx.shader, i++);
    }
    ctx.shader->setUniformInt("uNumLights", frameSnapshot.frame.lights.size());

    bool blending = false;

    for (auto& cmd : cmdList) {
        bool isTransparent = cmd.flags & DrawFlags::Transparent;

        if (isTransparent && !blending) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDepthMask(GL_FALSE);
            blending = true;
        }

        ctx.shader->setUniformMat4("model", cmd.model);
        cmd.material->bind(*ctx.shader);
        cmd.mesh->drawSubMesh(*cmd.submesh);
    }

    if (blending) {
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    }

    endScene();
}