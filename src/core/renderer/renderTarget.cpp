#include "core/renderer/renderTarget.hpp"
#include <iostream>

void RenderTarget::init(int w, int h, int samples) {
    width   = w;
    height  = h;
    this->samples = samples;
}

void RenderTarget::attachColor(GLenum attachment, TextureDesc desc) {
    colors.push_back({ attachment, desc, 0 });
}

void RenderTarget::attachDepth(TextureDesc desc) {
    depthDesc = desc;
    hasDepth  = true;
}

void RenderTarget::build() {
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    for (auto& c : colors) {
        glGenTextures(1, &c.tex);

        if (samples > 0) {
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, c.tex);
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,
                                    samples,
                                    c.desc.internalFormat,
                                    width, height,
                                    GL_TRUE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, c.attachment,
                                   GL_TEXTURE_2D_MULTISAMPLE, c.tex, 0);
        } else {
            glBindTexture(GL_TEXTURE_2D, c.tex);
            glTexImage2D(GL_TEXTURE_2D, 0,
                         c.desc.internalFormat,
                         width, height, 0,
                         c.desc.format, c.desc.type,
                         nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, c.attachment,
                                   GL_TEXTURE_2D, c.tex, 0);
        }
    }

    if (hasDepth) {
        glGenTextures(1, &depthTex);

        if (samples > 0) {
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, depthTex);
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,
                                    samples,
                                    depthDesc.internalFormat,
                                    width, height,
                                    GL_TRUE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                   GL_TEXTURE_2D_MULTISAMPLE, depthTex, 0);
        } else {
            glBindTexture(GL_TEXTURE_2D, depthTex);
            glTexImage2D(GL_TEXTURE_2D, 0,
                         depthDesc.internalFormat,
                         width, height, 0,
                         depthDesc.format, depthDesc.type,
                         nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                   GL_TEXTURE_2D, depthTex, 0);
        }
    }

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "RenderTarget incomplete: 0x" << std::hex << status << "\n";

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTarget::resize(int w, int h) {
    destroy();
    width  = w;
    height = h;
    build();
}

void RenderTarget::bindForDraw() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void RenderTarget::bindForRead() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
}

GLuint RenderTarget::getColor(int index) const {
    return colors[index].tex;
}

GLuint RenderTarget::getDepth() const {
    return depthTex;
}

void RenderTarget::resolveTo(RenderTarget& dst, GLbitfield mask) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dst.fbo);
    glBlitFramebuffer(
        0, 0, width, height,
        0, 0, dst.width, dst.height,
        mask, GL_NEAREST
    );
}

void RenderTarget::destroy() {
    for (auto& c : colors)
        if (c.tex) { glDeleteTextures(1, &c.tex); c.tex = 0; }
    if (hasDepth && depthTex) { glDeleteTextures(1, &depthTex); depthTex = 0; }
    if (fbo)  { glDeleteFramebuffers(1, &fbo); fbo = 0; }
}

RenderTarget::~RenderTarget() {
    destroy();
}