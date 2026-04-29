#include "core/scene/scene.hpp"

#include "core/renderer/mesh.hpp"
#include "core/resourceManager.hpp"
#include "core/scene/model.hpp"

#include "utils/objLoader.hpp"

#include <algorithm>
#include <iostream>

Scene::Scene() {
    this->rm = std::make_unique<ResourceManager>();
}

Scene::~Scene() {
    delete this->renderer;
    delete this->cam;
}

void Scene::createShader(const std::string& tag,
                      const std::string& vert,
                      const std::string& frag) {

    this->rm->shaders.create(tag, vert, frag);
}

/**
* @param objFileName Leave vertices and indices blank if you are using this
*/
void Scene::createMesh(const std::string& tag,
                      const std::vector<float>& vertices,
                      const std::vector<unsigned int> indices,
                      const std::string& objFileName) {


    if (vertices.empty() != true) {
        std::shared_ptr<Mesh> m = this->rm->meshes.create(tag, vertices, 8, indices);
        m->addVertexAttribute(0, 3, GL_FLOAT, 8 * sizeof(float), (const void*)0);
        m->addVertexAttribute(1, 3, GL_FLOAT, 8 * sizeof(float), (const void*)(sizeof(float) * 3));
        m->addVertexAttribute(2, 2, GL_FLOAT, 8 * sizeof(float), (const void*)(sizeof(float) * 6));
    } else if (objFileName.empty() != true) {
        MeshStructure meshStructure = loadOBJ(objFileName);
        std::shared_ptr<Mesh> m = this->rm->meshes.create(tag, meshStructure.vertices, 8, meshStructure.indices);
        m->addVertexAttribute(0, 3, GL_FLOAT, 8 * sizeof(float), (const void*)0);
        m->addVertexAttribute(1, 3, GL_FLOAT, 8 * sizeof(float), (const void*)(sizeof(float) * 3));
        m->addVertexAttribute(2, 2, GL_FLOAT, 8 * sizeof(float), (const void*)(sizeof(float) * 6));

        m->submeshes = meshStructure.submeshes;

        for (auto& sm : m->submeshes) {
            m->computeBounds(sm);
        }

        for (auto& [key, mat] : meshStructure.materials) {
            this->createMaterial(key, mat.shininess, mat.diffuse, mat.specular);
        }
    }
}

void Scene::createTexture(const std::string& tag, const std::string& path) {
    this->rm->textures.create(tag, path);
}

void Scene::createMaterial(const std::string& tag,
                           const float shininess,
                           std::shared_ptr<Texture> tex,
                           std::shared_ptr<Texture> spec) {
    std::shared_ptr<Material> mat = this->rm->materials.create(tag);
    mat->diffuse = tex;
    mat->specular = spec;
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
                          const std::string& meshTag) 
{
    std::shared_ptr<Mesh> m = this->rm->meshes.get(meshTag);

    std::unique_ptr<Model> model = std::make_unique<Model>(m);
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

Camera* Scene::createCamera(float fovInRadians,
                            float aspectRatio,
                            float zNear,
                            float zFar) {
    this->cam = new Camera(fovInRadians, aspectRatio, zNear, zFar);
    return this->cam;
}

Renderer* Scene::createRenderer() {
    this->renderer = new Renderer();

    return this->renderer;
}

std::shared_ptr<Shader> Scene::getShader(const std::string& tag) {
    return this->rm->shaders.get(tag);
}

std::shared_ptr<Mesh> Scene::getMesh(const std::string& tag) {
    return this->rm->meshes.get(tag);
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

Camera* Scene::getCamera() {
    /*
    auto it = this->cameras.find(tag);
    if (it == this->cameras.end())
        return nullptr;

    return it->second.get();
    */
    return this->cam;
}

Renderer* Scene::getRenderer() {
    return this->renderer;
}

static float computeTransparentDepth(
    const glm::mat4& model,
    const glm::mat4& view,
    const glm::vec3& localCenter = glm::vec3(0.0f)
) {
    glm::vec4 worldPos = model * glm::vec4(localCenter, 1.0f);
    glm::vec4 viewPos  = view * worldPos;

    return -viewPos.z; // depth along camera forward axis
}

FrameData Scene::buildFrame() {
    FrameData frame;

    const glm::mat4 view = this->getCamera()->getView();

    for (auto& [k, m] : models) {
        m->updateModelMatrix();

        for (auto& sm : m->mesh->submeshes) {
            auto mat = this->getMaterial(sm.materialName);
            if (!mat) continue;

            if (mat->opacity < 1.0f) {
                float dist = computeTransparentDepth(
                    m->getModelMatrix(),
                    view,
                    sm.boundsCenter
                );

                frame.transparent.push_back({
                    m->mesh.get(),
                    &sm,
                    mat,
                    m->getModelMatrix(),
                    dist
                });
            } else {
                frame.opaque.push_back({
                    m->mesh.get(),
                    &sm,
                    mat,
                    m->getModelMatrix()
                });
            }
        }
    }

    for (auto& [k, l] : lights)
        frame.lights.push_back(l.get());

    return frame;
}

RenderContext Scene::buildContext(std::shared_ptr<Shader> shader) {
    Camera* cam = this->getCamera();

    RenderContext ctx{};
    ctx.cameraPosition = cam->position;
    ctx.projection = cam->getProjection();
    ctx.view = cam->getView();
    ctx.shader = shader;

    return ctx;
}

FrameSnapshot Scene::buildSnapshot(const std::string& shaderTag) {
    return {this->buildFrame(), this->buildContext(this->getShader(shaderTag))};
}