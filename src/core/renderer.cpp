#include "core/renderer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/shader.hpp"
#include "postprocess_pass.hpp"
#include "utils/fileUtils.hpp"

#include "core/mesh.hpp"
#include "core/postprocess_pass.hpp"

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

void Renderer::addPass(const PostProcessPass& pass) {
    this->passes.push_back(pass);
}

void Renderer::clearPasses() {
    this->passes.clear();
}

static GLuint makeTexture(int width, int height, GLenum internalFmt, GLenum fmt, GLenum type, GLenum filter, GLenum wrap = GL_NONE) {
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFmt, width, height, 0, fmt, type, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    if (wrap != GL_NONE) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    }
    return tex;
}

static GLuint makeMSAATexture(int width, int height, GLenum internalFmt, int samples) {
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, tex);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFmt, width, height, GL_TRUE);
    return tex;
}

static GLuint makeFramebuffer(std::initializer_list<std::pair<GLenum, GLuint>> attachments) {
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    for (auto& [point, tex] : attachments)
        glFramebufferTexture2D(GL_FRAMEBUFFER, point, GL_TEXTURE_2D, tex, 0);
    return fbo;
}

static GLuint makeMSAAFramebuffer(std::initializer_list<std::pair<GLenum, GLuint>> attachments) {
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    for (auto& [point, tex] : attachments)
        glFramebufferTexture2D(GL_FRAMEBUFFER, point, GL_TEXTURE_2D_MULTISAMPLE, tex, 0);
    return fbo;
}

void Renderer::init(int width, int height) {
    this->width  = width;
    this->height = height;

    // --- MSAA G-buffer ---
    msaaColorTex  = makeMSAATexture(width, height, GL_RGB16F, msaaSamples);
    msaaDepthTex  = makeMSAATexture(width, height, GL_DEPTH_COMPONENT24, msaaSamples);
    msaaNormalTex = makeMSAATexture(width, height, GL_RGB16F, msaaSamples);

    msaaFBO = makeMSAAFramebuffer({
        { GL_COLOR_ATTACHMENT0, msaaColorTex  },
        { GL_DEPTH_ATTACHMENT,  msaaDepthTex  },
        { GL_COLOR_ATTACHMENT1, msaaNormalTex },
    });

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "MSAA FBO incomplete\n";

    const GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, drawBuffers);

    // --- Resolve G-buffer (regular, for post-process sampling) ---
    colorTex  = makeTexture(width, height, GL_RGB16F, GL_RGB, GL_FLOAT, GL_LINEAR);
    depthTex  = makeTexture(width, height, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT, GL_NEAREST, GL_CLAMP_TO_EDGE);
    normalTex = makeTexture(width, height, GL_RGB16F, GL_RGB, GL_FLOAT, GL_NEAREST, GL_CLAMP_TO_EDGE);

    FBO = makeFramebuffer({
        { GL_COLOR_ATTACHMENT0, colorTex  },
        { GL_DEPTH_ATTACHMENT,  depthTex  },
        { GL_COLOR_ATTACHMENT1, normalTex },
    });

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Resolve FBO incomplete\n";

    glDrawBuffers(2, drawBuffers);

    // --- Ping-pong buffers ---
    pingTex = makeTexture(width, height, GL_RGB16F, GL_RGB, GL_FLOAT, GL_LINEAR);
    pongTex = makeTexture(width, height, GL_RGB16F, GL_RGB, GL_FLOAT, GL_LINEAR);

    pingFBO = makeFramebuffer({ { GL_COLOR_ATTACHMENT0, pingTex } });
    pongFBO = makeFramebuffer({ { GL_COLOR_ATTACHMENT0, pongTex } });

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    initFullscreenQuad();
    blitShader = new Shader("blit.vert", "blit.frag");
}

void Renderer::beginScene() {
    glBindFramebuffer(GL_FRAMEBUFFER, msaaFBO);
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::endScene() {
    // --- Resolve MSAA into regular FBO ---
    glBindFramebuffer(GL_READ_FRAMEBUFFER, msaaFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);

    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    glReadBuffer(GL_COLOR_ATTACHMENT1);
    glDrawBuffer(GL_COLOR_ATTACHMENT1);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

    // --- Post-process chain ---
    glDisable(GL_DEPTH_TEST);

    struct PingPong {
        GLuint fbo, tex;
    } buffers[2] = { {pingFBO, pingTex}, {pongFBO, pongTex} };
    int target = 0;

    unsigned int inputTex = this->colorTex;

    auto bindTex = [](int slot, GLuint tex) {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, tex);
    };

    std::unordered_map<std::string, GLuint> snapshots;

    snapshots["scene"] = this->colorTex;

    for (auto& pass : passes) {
        glBindFramebuffer(GL_FRAMEBUFFER, buffers[target].fbo);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        pass.shader->bind();
        bindTex(0, inputTex);  pass.shader->setUniformInt("uTexture", 0);
        bindTex(1, depthTex);  pass.shader->setUniformInt("uDepth",   1);
        bindTex(2, normalTex); pass.shader->setUniformInt("uNormal",  2);
        pass.shader->setUniformVec2("uOutputSize", glm::vec2(width, height));
        pass.applyUniforms();

        int slot = 3;
        for (auto& [uniformName, snapshotName] : pass.extraTextures) {
            if (snapshots.count(snapshotName)) {
                bindTex(slot, snapshots[snapshotName]);
                pass.shader->setUniformInt(uniformName, slot);
                slot++;
            } else {
                std::cout << "MISSING snapshot: " << snapshotName << "\n";
            }
        }

        renderFullscreenQuad();

        inputTex = buffers[target].tex;
        target ^= 1;

        if (!pass.saveOutputAs.empty()) {
            snapshots[pass.saveOutputAs] = inputTex;
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    blitShader->bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inputTex);
    blitShader->setUniformInt("uTexture", 0);

    renderFullscreenQuad();

    glEnable(GL_DEPTH_TEST);
}

void Renderer::setDimensions(int width, int height) {
    this->width = width;
    this->height = height;

    // --- resize MSAA ---
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msaaColorTex);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaaSamples, GL_RGB16F, width, height, GL_TRUE);

    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msaaDepthTex);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaaSamples, GL_DEPTH_COMPONENT24, width, height, GL_TRUE);

    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msaaNormalTex);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaaSamples, GL_RGB16F, width, height, GL_TRUE);

    // --- resize resolve ---
    glBindTexture(GL_TEXTURE_2D, colorTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);

    glBindTexture(GL_TEXTURE_2D, depthTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glBindTexture(GL_TEXTURE_2D, normalTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);

    // --- resize ping-pong ---
    glBindTexture(GL_TEXTURE_2D, pingTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);

    glBindTexture(GL_TEXTURE_2D, pongTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);

    glBindTexture(GL_TEXTURE_2D, 0);
}