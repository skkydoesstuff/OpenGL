#pragma once

#include <glad/glad.h>
#include <vector>

struct TextureDesc {
    GLenum internalFormat;
    GLenum format;
    GLenum type;
};

class RenderTarget {
public:
    void init(int w, int h, int samples = 0);
    void attachColor(GLenum attachment, TextureDesc desc);
    void attachDepth(TextureDesc desc);
    void build();

    void resize(int w, int h);

    void bindForDraw();
    void bindForRead();

    GLuint getColor(int index = 0) const;
    GLuint getDepth() const;

    int getWidth()  const { return width; }
    int getHeight() const { return height; }

    GLuint fbo = 0;

    void resolveTo(RenderTarget& dst, GLbitfield mask);
    void destroy();

    ~RenderTarget();

private:
    int width = 0, height = 0;
    int samples = 0;

    struct ColorAttachment {
        GLenum attachment;
        TextureDesc desc;
        GLuint tex = 0;
    };

    std::vector<ColorAttachment> colors;

    TextureDesc depthDesc;
    GLuint depthTex = 0;

    bool hasDepth = false;
};