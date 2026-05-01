#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "core/resourceManager.hpp"
#include "core/scene/model.hpp"
#include "core/scene/light.hpp"
#include "core/scene/camera.hpp"
#include "core/renderer/renderer.hpp"
#include "core/renderer/postprocess_pass.hpp"
#include "core/renderer/material.hpp"
#include "core/renderer/frameData.hpp"

class Scene {
public:
    Scene(uint32_t width, uint32_t height);

    void loadSceneFromJSON(const std::string& jsonPath);
    void saveSceneToJSON(const std::string& jsonPath);

    void createShader(const std::string& tag,
                      const std::string& vert,
                      const std::string& frag);

    void createMesh(const std::string& tag,
                    const std::vector<float>& vertices = {},
                    const std::vector<unsigned int> indices = {},
                    const std::string& objFileName = "");

    void createTexture(const std::string& tag, const std::string& path);

    void createMaterial(const std::string& tag,
                        const float shininess,
                        std::shared_ptr<Texture> tex,
                        std::shared_ptr<Texture> spec,
                        std::shared_ptr<Texture> opacity,
                        std::shared_ptr<Texture> normal);

    void createPass(const std::string& tag,
                    const std::string& shaderTag,
                    std::unordered_map<std::string, UniformValue> uniforms = {},
                    std::unordered_map<std::string, std::string> extraTextures = {},
                    const std::string& output = "");
        
    Model* createModel(const std::string& tag,
                       const std::string& meshTag);

    Light* createLight(const std::string& tag);

    Camera* createCamera(float fovInRadians,
                         float aspectRatio,
                         float zNear,
                         float zFar);
    
    std::shared_ptr<Shader> getShader(const std::string& tag);
    std::shared_ptr<Mesh> getMesh(const std::string& tag);
    std::shared_ptr<Texture> getTexture(const std::string& tag);
    std::shared_ptr<Material> getMaterial(const std::string& tag);
    PostProcessPass* getPass(const std::string& tag);
    Model* getModel(const std::string& tag);
    Light* getLight(const std::string& tag);
    
    Camera* getCamera();
    Renderer* getRenderer();

    FrameSnapshot buildSnapshot(const std::string& shaderTag);

    ~Scene() = default;
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene &) = delete;
private:
    std::unique_ptr<ResourceManager> rm;

    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Camera> cam;

    FrameData buildFrame();
    RenderContext buildContext(std::shared_ptr<Shader> shader);
};