#include "app/app.hpp"
#include <memory>

#include <GLFW/glfw3.h>

void App::logicLoop() {
    std::shared_ptr<Renderer> r = this->rm.renderers.get("main");

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

    if (fbWidth != r->getWidth() || fbHeight != r->getHeight()) {
        r->setDimensions(fbWidth, fbHeight);
        glViewport(0, 0, fbWidth, fbHeight);
    }

    r->clearPasses();
    if (this->scanlineOn == true) {
        scanline.uniforms["uScanlineThickness"] = scanThickness;
        scanline.uniforms["uScanlineDarkness"]  = scanDarkness;
        scanline.uniforms["uPhosphorStrength"]  = phosphorStrength;
        scanline.uniforms["uGlowStrength"]      = glowStrength;
        scanline.uniforms["uVignetteStrength"]  = vignetteStrength;
        scanline.uniforms["uBrightBoost"]       = brightBoost;

        r->addPass(scanline);
    }

    if (this->outlineOn == true) {
        outline.uniforms["uNormalThreshold"] = normThresh;
        outline.uniforms["uEdgeStrength"] = edgeStrength;
        outline.uniforms["uDepthThreshold"] = depthThresh;
        outline.uniforms["uEdgeWidth"] = edgeWidth;

        r->addPass(outline);
    }

    if (this->pixelatedOn == true) {
        pixelated.uniforms["uPixelSize"] = pixelSize;
        r->addPass(pixelated);
    }
}