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

    if (this->settings.bloomOn == true) {
        auto threshold = this->rm.passes.get("threshold");        
        auto blurH = this->rm.passes.get("blurH");        
        auto blurV = this->rm.passes.get("blurV");        
        auto composite = this->rm.passes.get("composite");        

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
        auto outline = this->rm.passes.get("outline");

        outline->uniforms["uNormalThreshold"] = this->settings.normThresh;
        outline->uniforms["uEdgeStrength"]    = this->settings.edgeStrength;
        outline->uniforms["uDepthThreshold"]  = this->settings.depthThresh;
        outline->uniforms["uEdgeWidth"]       = this->settings.edgeWidth;

        r->addPass(outline);
    }

    if (this->settings.scanlineOn == true) {
        auto scanline = this->rm.passes.get("scanline");

        scanline->uniforms["uScanlineThickness"] = this->settings.scanThickness;
        scanline->uniforms["uScanlineDarkness"]  = this->settings.scanDarkness;
        scanline->uniforms["uPhosphorStrength"]  = this->settings.phosphorStrength;
        scanline->uniforms["uGlowStrength"]      = this->settings.glowStrength;
        scanline->uniforms["uVignetteStrength"]  = this->settings.vignetteStrength;
        scanline->uniforms["uBrightBoost"]       = this->settings.brightBoost;

        r->addPass(scanline);
    }

    if (this->settings.pixelatedOn == true) {
        auto pixelated = this->rm.passes.get("pixelated");

        pixelated->uniforms["uPixelSize"] = this->settings.pixelSize;
        r->addPass(pixelated);
    }
}