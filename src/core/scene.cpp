#include "core/scene.hpp"

#include "core/resourceManager.hpp"
#include "core/model.hpp"

Scene::Scene() {
    this->rm = std::make_unique<ResourceManager>();
}

void Scene::createShader(const std::string& tag,
                      const std::string& vert,
                      const std::string& frag) {

    this->rm->shaders.create(tag, vert, frag);
}

void Scene::createMesh(const std::string& tag,
                      const std::vector<float>& vertices,
                      const std::vector<unsigned int> indices) {

    std::shared_ptr<Mesh> m = this->rm->meshes.create(tag, vertices, 8, indices);
    m->addVertexAttribute(0, 3, GL_FLOAT, 8 * sizeof(float), (const void*)0);
    m->addVertexAttribute(1, 3, GL_FLOAT, 8 * sizeof(float), (const void*)(sizeof(float) * 3));
    m->addVertexAttribute(2, 2, GL_FLOAT, 8 * sizeof(float), (const void*)(sizeof(float) * 6));
}

void Scene::createTexture(const std::string& tag, const std::string& path) {
    this->rm->textures.create(tag, path);
}

void Scene::createMaterial(const std::string& tag,
                           const float shininess,
                           const std::string& tex,
                           const std::string& spec) {
    std::shared_ptr<Material> mat = this->rm->materials.create(tag);
    mat->diffuse = this->getTexture(tex);
    mat->specular = this->getTexture(spec);
    mat->shininess = shininess;
}

void Scene::createPass(const std::string& tag,
                       std::shared_ptr<Shader> shader,
                       std::unordered_map<std::string, UniformValue> uniforms,
                       std::unordered_map<std::string, std::string> extraTextures,
                       const std::string& output) {
    
    std::shared_ptr<PostProcessPass> pass = this->rm->passes.create(tag);
    pass->shader = shader;
    pass->uniforms = std::move(uniforms);
    pass->extraTextures = std::move(extraTextures);
    pass->saveOutputAs = std::move(output);
}

Model* Scene::createModel(const std::string& tag,
                          const std::string& shaderTag,
                          const std::string& meshTag) {
    std::shared_ptr<Shader> s = this->rm->shaders.get(shaderTag);
    std::shared_ptr<Mesh> m = this->rm->meshes.get(meshTag);
    
    std::unique_ptr<Model> model = std::make_unique<Model>(s, m);
    Model* ptr = model.get();

    models[tag] = std::move(model);

    return ptr;
}

Light* Scene::createLight(const std::string& tag) {
    std::unique_ptr<Light> light = std::make_unique<Light>();
    Light* ptr = light.get();

    lights[tag] = std::move(light);

    return ptr;
}

Camera* Scene::createCamera(const std::string& tag,
                            float fovInRadians,
                            float aspectRatio,
                            float zNear,
                            float zFar) {

    std::unique_ptr<Camera> camera = std::make_unique<Camera>(fovInRadians, aspectRatio, zNear, zFar);
    Camera* ptr = camera.get();

    cameras[tag] = std::move(camera);

    return ptr;
}

Renderer* Scene::createRenderer(const std::string& tag) {

    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>();
    Renderer* ptr = renderer.get();

    renderers[tag] = std::move(renderer);

    return ptr;
}

std::shared_ptr<Shader> Scene::getShader(const std::string& tag) {
    return this->rm->shaders.get(tag);
}

std::shared_ptr<Texture> Scene::getTexture(const std::string& tag) {
    return this->rm->textures.get(tag);
}

std::shared_ptr<Material> Scene::getMaterial(const std::string& tag) {
    return this->rm->materials.get(tag);
}

std::shared_ptr<PostProcessPass> Scene::getPass(const std::string& tag) {
    return this->rm->passes.get(tag);
}

Model* Scene::getModel(const std::string& tag) {
    auto it = this->models.find(tag);
    if (it == this->models.end())
        return nullptr;

    return it->second.get();
}

Light* Scene::getLight(const std::string& tag) {
    auto it = this->lights.find(tag);
    if (it == this->lights.end())
        return nullptr;

    return it->second.get();
}

Camera* Scene::getCamera(const std::string& tag) {
    auto it = this->cameras.find(tag);
    if (it == this->cameras.end())
        return nullptr;

    return it->second.get();
}

Renderer* Scene::getRenderer(const std::string& tag) {
    auto it = this->renderers.find(tag);
    if (it == this->renderers.end())
        return nullptr;

    return it->second.get();
}
