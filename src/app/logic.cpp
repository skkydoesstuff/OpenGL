#include "app/app.hpp"
#include <memory>

#include <GLFW/glfw3.h>

void App::logicLoop() {
    Renderer* r = scene.getRenderer("main");

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

    if (fbWidth != r->getWidth() || fbHeight != r->getHeight()) {
        r->setDimensions(fbWidth, fbHeight);
        glViewport(0, 0, fbWidth, fbHeight);
    }

    r->clearPasses();

    if (this->settings.bloomOn == true) {
        auto threshold = scene.getPass("threshold");        
        auto blurH = scene.getPass("blurH");        
        auto blurV = scene.getPass("blurV");        
        auto composite = scene.getPass("composite");        

        threshold->uniforms["uThreshold"] = this->settings.thresholdVal;
        threshold->saveOutputAs = "bloomThreshold";
        composite->uniforms["uIntensity"] = this->settings.intensity;

        r->addPass(threshold);
        for (int i = 0; i < this->settings.blurPasses; i++) {
            r->addPass(blurH);
            r->addPass(blurV);
        }
        r->addPass(composite);
    }

    if (this->settings.outlineOn == true) {
        auto outline = scene.getPass("outline");

        outline->uniforms["uNormalThreshold"] = this->settings.normThresh;
        outline->uniforms["uEdgeStrength"]    = this->settings.edgeStrength;
        outline->uniforms["uDepthThreshold"]  = this->settings.depthThresh;
        outline->uniforms["uEdgeWidth"]       = this->settings.edgeWidth;

        r->addPass(outline);
    }

    if (this->settings.scanlineOn == true) {
        auto scanline = scene.getPass("scanline");

        scanline->uniforms["uScanlineThickness"] = this->settings.scanThickness;
        scanline->uniforms["uScanlineDarkness"]  = this->settings.scanDarkness;
        scanline->uniforms["uPhosphorStrength"]  = this->settings.phosphorStrength;
        scanline->uniforms["uGlowStrength"]      = this->settings.glowStrength;
        scanline->uniforms["uVignetteStrength"]  = this->settings.vignetteStrength;
        scanline->uniforms["uBrightBoost"]       = this->settings.brightBoost;

        r->addPass(scanline);
    }

    if (this->settings.pixelatedOn == true) {
        auto pixelated = scene.getPass("pixelated");

        pixelated->uniforms["uPixelSize"] = this->settings.pixelSize;
        r->addPass(pixelated);
    }
}