#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "core/resourceManager.hpp"
#include "core/model.hpp"
#include "core/light.hpp"
#include "core/camera.hpp"
#include "core/renderer/renderer.hpp"
#include "core/renderer/postprocess_pass.hpp"
#include "material.hpp"

class Scene {
public:
    Scene();

    void createShader(const std::string& tag,
                      const std::string& vert,
                      const std::string& frag);

    void createMesh(const std::string& tag,
                    const std::vector<float>& vertices,
                    const std::vector<unsigned int> indices = {});

    void createTexture(const std::string& tag, const std::string& path);

    void createMaterial(const std::string& tag,
                        const float shininess,
                        const std::string& tex,
                        const std::string& spec = "");

    void createPass(const std::string& tag,
                    std::shared_ptr<Shader> shader,
                    std::unordered_map<std::string, UniformValue> uniforms = {},
                    std::unordered_map<std::string, std::string> extraTextures = {},
                    const std::string& output = "");

    Model* createModel(const std::string& tag,
                       const std::string& shaderTag,
                       const std::string& meshTag);

    Light* createLight(const std::string& tag);

    Camera* createCamera(float fovInRadians,
                         float aspectRatio,
                         float zNear,
                         float zFar);
                       
    Renderer* createRenderer(const std::string& tag);
    
    std::shared_ptr<Shader> getShader(const std::string& tag);
    std::shared_ptr<Texture> getTexture(const std::string& tag);
    std::shared_ptr<Material> getMaterial(const std::string& tag);
    std::shared_ptr<PostProcessPass> getPass(const std::string& tag);

    Model* getModel(const std::string& tag);
    Light* getLight(const std::string& tag);
    Camera* getCamera();
    Renderer* getRenderer(const std::string& tag);

    int getLightCount();
    void uploadCameraData(const std::string& shaderTag);
    void uploadLightData(const std::string& shaderTag);

    void drawScene(const std::string& shaderTag);

    ~Scene() = default;
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene &) = delete;
private:
    std::unique_ptr<ResourceManager> rm;
    std::unordered_map<std::string, std::unique_ptr<Model>> models;
    std::unordered_map<std::string, std::unique_ptr<Light>> lights;
    //std::unordered_map<std::string, std::unique_ptr<Camera>> cameras;
    std::unordered_map<std::string, std::unique_ptr<Renderer>> renderers;

    Camera* cam;
};