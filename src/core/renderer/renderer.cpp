#include "core/renderer/renderer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/shader.hpp"

#include "utils/fileUtils.hpp"
#include "utils/debugUtils.hpp"

#include "core/mesh.hpp"
#include "core/renderer/postprocess_pass.hpp"

#include <iostream>

Renderer::~Renderer() {
    delete this->fullscreenQuadMesh;
    delete this->blitShader;
}

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

    this->fullscreenQuadMesh = new Mesh(quadVertices, 4);
    this->fullscreenQuadMesh->addVertexAttribute(0, 2, GL_FLOAT, 4 * sizeof(float), (void*)0);
    this->fullscreenQuadMesh->addVertexAttribute(1, 2, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void Renderer::renderFullscreenQuad() {
    this->fullscreenQuadMesh->draw();
}

void Renderer::addPass(std::shared_ptr<PostProcessPass> pass) {
    this->passes.push_back(pass);
}

void Renderer::clearPasses() {
    this->passes.clear();
}


void Renderer::init(int width, int height) {
    this->width  = width;
    this->height = height;

    msaa.init(width, height, msaaSamples);
    msaa.attachColor(GL_COLOR_ATTACHMENT0, {GL_RGB16F, GL_RGB, GL_FLOAT});
    msaa.attachColor(GL_COLOR_ATTACHMENT1, {GL_RGB16F, GL_RGB, GL_FLOAT});
    msaa.attachDepth({GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT});
    msaa.build();

    scene.init(width, height, 0);
    scene.attachColor(GL_COLOR_ATTACHMENT0, {GL_RGB16F, GL_RGB, GL_FLOAT});
    scene.attachColor(GL_COLOR_ATTACHMENT1, {GL_RGB16F, GL_RGB, GL_FLOAT});
    scene.attachDepth({GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT});
    scene.build();

    ping.init(width, height, 0);
    ping.attachColor(GL_COLOR_ATTACHMENT0, {GL_RGB16F, GL_RGB, GL_FLOAT});
    ping.attachColor(GL_COLOR_ATTACHMENT1, {GL_RGB16F, GL_RGB, GL_FLOAT});
    ping.attachDepth({GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT});
    ping.build();

    pong.init(width, height, 0);
    pong.attachColor(GL_COLOR_ATTACHMENT0, {GL_RGB16F, GL_RGB, GL_FLOAT});
    pong.attachColor(GL_COLOR_ATTACHMENT1, {GL_RGB16F, GL_RGB, GL_FLOAT});
    pong.attachDepth({GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT});
    pong.build();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    initFullscreenQuad();
    blitShader = new Shader("blit.vert", "blit.frag");
}

void Renderer::beginScene() {
    msaa.bindForDraw();
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::endScene() {
    // -----------------------------
    // 1. Resolve MSAA -> scene RT
    // -----------------------------
    msaa.resolveTo(scene, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);

    // -----------------------------
    // 2. Snapshot system
    // -----------------------------
    std::unordered_map<std::string, GLuint> snapshots;
    snapshots["scene"] = scene.getColor(0);

    RenderTarget* input = &scene;
    RenderTarget* output = &ping;

    auto bindTex = [](int slot, GLuint tex) {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, tex);
    };

    // -----------------------------
    // 3. Post-process chain
    // -----------------------------
    for (auto& pass : passes) {

        output->bindForDraw();
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        pass->shader->bind();

        // core buffers
        bindTex(0, input->getColor(0));
        pass->shader->setUniformInt("uTexture", 0);

        bindTex(1, scene.getDepth());
        pass->shader->setUniformInt("uDepth", 1);

        bindTex(2, scene.getColor(1));
        pass->shader->setUniformInt("uNormal", 2);

        pass->shader->setUniformVec2("uOutputSize", glm::vec2(width, height));

        pass->applyUniforms();

        // extra snapshots
        int slot = 3;
        for (auto& [uniformName, snapshotName] : pass->extraTextures) {
            auto it = snapshots.find(snapshotName);
            if (it != snapshots.end()) {
                bindTex(slot, it->second);
                pass->shader->setUniformInt(uniformName, slot);
                slot++;
            } else {
                DEBUG_PRINT(std::cout << "MISSING snapshot: " << snapshotName << "\n");
            }
        }

        renderFullscreenQuad();

        // store snapshot if requested
        if (!pass->saveOutputAs.empty()) {
            snapshots[pass->saveOutputAs] = output->getColor(0);
        }

        std::swap(input, output);
    }

    // -----------------------------
    // 4. Final blit to screen
    // -----------------------------
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    blitShader->bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, input->getColor(0));
    blitShader->setUniformInt("uTexture", 0);

    renderFullscreenQuad();

    glEnable(GL_DEPTH_TEST);
}

void Renderer::setDimensions(int width, int height) {
    this->width = width;
    this->height = height;

    // --- resize MSAA ---
    msaa.resize(width, height);
    scene.resize(width, height);
    ping.resize(width, height);
    pong.resize(width, height);

    glBindTexture(GL_TEXTURE_2D, 0);
}