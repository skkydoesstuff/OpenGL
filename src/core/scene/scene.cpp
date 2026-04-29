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
            this->createMaterial(key, mat->shininess, mat->diffuse, mat->specular);
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
    
    PostProcessPass* pass = this->rm->passes.create(tag);
    pass->shader = shader;
    pass->uniforms = std::move(uniforms);
    pass->extraTextures = std::move(extraTextures);
    pass->saveOutputAs = std::move(output);
}

Model* Scene::createModel(const std::string& tag,
                          const std::string& meshTag) 
{
    std::shared_ptr<Mesh> m = this->rm->meshes.get(meshTag);

    return this->rm->models.create(tag, m);
}

Light* Scene::createLight(const std::string& tag) {
    return this->rm->lights.create(tag);
}

Camera* Scene::createCamera(float fovInRadians,
                            float aspectRatio,
                            float zNear,
                            float zFar) {
    this->cam = std::make_unique<Camera>(fovInRadians, aspectRatio, zNear, zFar);
    return this->cam.get();
}

Renderer* Scene::createRenderer() {
    this->renderer = std::make_unique<Renderer>();

    return this->renderer.get();
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

PostProcessPass* Scene::getPass(const std::string& tag) {
    return this->rm->passes.get(tag);
}

Model* Scene::getModel(const std::string& tag) {
    return this->rm->models.get(tag);
}

Light* Scene::getLight(const std::string& tag) {
    return this->rm->lights.get(tag);
}

Camera* Scene::getCamera() {
    return this->cam.get();
}

Renderer* Scene::getRenderer() {
    return this->renderer.get();
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

    const glm::mat4 view = getCamera()->getView();

    for (Model* m : rm->models.values()) {
        m->updateModelMatrix();

        glm::mat4 model = m->getModelMatrix();

        for (SubMesh& sm : m->mesh->submeshes) {
            Material* mat = sm.material;
            if (!mat) continue;

            DrawCommand cmd;
            cmd.mesh = m->mesh.get();
            cmd.submesh = &sm;
            cmd.material = mat;
            cmd.model = model;

            if (sm.renderType == RenderType::Transparent) {
                cmd.flags = Transparent;
                cmd.depth = computeTransparentDepth(model, view, sm.boundsCenter);
            } else {
                cmd.flags = Opaque;
            }

            frame.commands.push_back(cmd);
        }
    }

    for (Light* l : rm->lights.values())
        frame.lights.push_back(l);

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