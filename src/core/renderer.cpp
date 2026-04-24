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

static GLuint makeFramebuffer(std::initializer_list<std::pair<GLenum, GLuint>> attachments) {
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    for (auto& [point, tex] : attachments)
        glFramebufferTexture2D(GL_FRAMEBUFFER, point, GL_TEXTURE_2D, tex, 0);
    return fbo;
}

void Renderer::init(int width, int height) {
    this->width  = width;
    this->height = height;

    // --- G-buffer ---
    colorTex  = makeTexture(width, height, GL_RGB16F, GL_RGB, GL_UNSIGNED_BYTE, GL_LINEAR);
    depthTex  = makeTexture(width, height, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT, GL_NEAREST, GL_CLAMP_TO_EDGE);
    normalTex = makeTexture(width, height, GL_RGB16F, GL_RGB, GL_FLOAT, GL_NEAREST, GL_CLAMP_TO_EDGE);

    FBO = makeFramebuffer({
        { GL_COLOR_ATTACHMENT0, colorTex  },
        { GL_DEPTH_ATTACHMENT,  depthTex  },
        { GL_COLOR_ATTACHMENT1, normalTex },
    });

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "G-buffer FBO incomplete\n";

    const GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, drawBuffers);

    // --- Ping-pong buffers ---
    pingTex = makeTexture(width, height, GL_RGB16F, GL_RGB, GL_UNSIGNED_BYTE, GL_LINEAR);
    pongTex = makeTexture(width, height, GL_RGB16F, GL_RGB, GL_UNSIGNED_BYTE, GL_LINEAR);

    pingFBO = makeFramebuffer({ { GL_COLOR_ATTACHMENT0, pingTex } });
    pongFBO = makeFramebuffer({ { GL_COLOR_ATTACHMENT0, pongTex } });

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    initFullscreenQuad();
    blitShader = new Shader("blit.vert", "blit.frag");
}

void Renderer::beginScene() {
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::endScene() {
    glDisable(GL_DEPTH_TEST);

    unsigned int fbos[2]  = { pingFBO,  pongFBO  };
    unsigned int texs[2]  = { pingTex,  pongTex  };

    unsigned int inputTex = this->colorTex;
    int target = 0;

    for (size_t i = 0; i < passes.size(); i++) {
        auto& pass = passes[i];

        glBindFramebuffer(GL_FRAMEBUFFER, fbos[target]);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        pass.shader->bind();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, inputTex);
        pass.shader->setUniformInt("uTexture", 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthTex);
        pass.shader->setUniformInt("uDepth", 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, normalTex);
        pass.shader->setUniformInt("uNormal", 2);

        pass.shader->setUniformVec2("uOutputSize", glm::vec2(width, height));

        for (auto& [name, value] : pass.uniforms) {
            std::visit([&](auto&& v) {
                using T = std::decay_t<decltype(v)>;
                if constexpr (std::is_same_v<T, int>)            pass.shader->setUniformInt(name, v);
                else if constexpr (std::is_same_v<T, float>)     pass.shader->setUniformFloat(name, v);
                else if constexpr (std::is_same_v<T, glm::vec2>) pass.shader->setUniformVec2(name, v);
                else if constexpr (std::is_same_v<T, glm::vec3>) pass.shader->setUniformVec3(name, v);
                else if constexpr (std::is_same_v<T, glm::vec4>) pass.shader->setUniformVec4(name, v);
                else if constexpr (std::is_same_v<T, glm::mat4>) pass.shader->setUniformMat4(name, v);
            }, value);
        }

        renderFullscreenQuad();

        inputTex = texs[target];
        target   = 1 - target;
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

    // --- resize color ---
    glBindTexture(GL_TEXTURE_2D, colorTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);

    // --- resize depth ---
    glBindTexture(GL_TEXTURE_2D, depthTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    // --- resize normal ---
    glBindTexture(GL_TEXTURE_2D, normalTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);

    // --- resize ping ---
    glBindTexture(GL_TEXTURE_2D, pingTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);

    // --- resize pong ---
    glBindTexture(GL_TEXTURE_2D, pongTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);

    glBindTexture(GL_TEXTURE_2D, 0);
}