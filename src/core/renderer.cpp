#include "core/renderer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "postprocess_pass.hpp"
#include "utils/fileUtils.hpp"

#include "core/mesh.hpp"
#include "core/postprocess_pass.hpp"

#include <iostream>

Renderer::~Renderer() {
    delete this->fullscreenQuadMesh;
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

void Renderer::init(int width, int height) {
    glGenFramebuffers(1, &this->FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);

    glGenTextures(1, &this->colorTex);
    glBindTexture(GL_TEXTURE_2D, this->colorTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex, 0);

    glGenTextures(1, &depthTex);
    glBindTexture(GL_TEXTURE_2D, depthTex);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_DEPTH_COMPONENT24,
        width,
        height,
        0,
        GL_DEPTH_COMPONENT,
        GL_FLOAT,
        nullptr
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D,
        depthTex,
        0
    );

    glGenTextures(1, &normalTex);
    glBindTexture(GL_TEXTURE_2D, normalTex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);    

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT1,
        GL_TEXTURE_2D,
        normalTex,
        0
    );

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "FBO NOT COMPLETE\n";
    }

    GLenum buffers[2] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1
    };
    glDrawBuffers(2, buffers);

    // ping
    glGenFramebuffers(1, &pingFBO);
    glGenTextures(1, &pingTex);

    glBindTexture(GL_TEXTURE_2D, pingTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, pingFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingTex, 0);

    // pong
    glGenFramebuffers(1, &pongFBO);
    glGenTextures(1, &pongTex);

    glBindTexture(GL_TEXTURE_2D, pongTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, pongFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pongTex, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    this->width = width;
    this->height = height;

    initFullscreenQuad();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

    unsigned int inputTex = this->colorTex;
    bool horizontal = true;

    for (size_t i = 0; i < passes.size(); i++) {
        auto& pass = passes[i];

        unsigned int targetFBO = horizontal ? pingFBO : pongFBO;
        unsigned int outputTex = horizontal ? pingTex : pongTex;

        glBindFramebuffer(GL_FRAMEBUFFER, targetFBO);
        glClear(GL_COLOR_BUFFER_BIT);

        pass.shader->bind();

        // bind input texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, inputTex);
        pass.shader->setUniformInt("uTexture", 0);
        
        // bind depth texture
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthTex);
        pass.shader->setUniformInt("uDepth", 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, normalTex);
        pass.shader->setUniformInt("uNormal", 2);

        // common uniforms
        pass.shader->setUniformVec2("uOutputSize", glm::vec2(width, height));
        pass.shader->setUniformMat4("MVPMatrix", glm::mat4(1.0f));

        // dynamic uniforms
        for (auto& [name, value] : pass.uniforms) {
            std::visit([&](auto&& v) {
                using T = std::decay_t<decltype(v)>;

                if constexpr (std::is_same_v<T, int>)
                    pass.shader->setUniformInt(name, v);
                else if constexpr (std::is_same_v<T, float>)
                    pass.shader->setUniformFloat(name, v);
                else if constexpr (std::is_same_v<T, glm::vec2>)
                    pass.shader->setUniformVec2(name, v);
                else if constexpr (std::is_same_v<T, glm::vec3>)
                    pass.shader->setUniformVec3(name, v);
                else if constexpr (std::is_same_v<T, glm::vec4>)
                    pass.shader->setUniformVec4(name, v);
                else if constexpr (std::is_same_v<T, glm::mat4>)
                    pass.shader->setUniformMat4(name, v);
            }, value);
        }
        
        renderFullscreenQuad();

        inputTex = outputTex;
        horizontal = !horizontal;
    }

    // final pass to screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, inputTex);
    renderFullscreenQuad();

    glEnable(GL_DEPTH_TEST);
}