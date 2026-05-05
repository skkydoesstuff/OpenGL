#include "core/scene/scene.hpp"

#include "core/renderer/mesh.hpp"
#include "core/resourceManager.hpp"
#include "core/scene/objects/model.hpp"

#include "utils/objLoader.hpp"
#include "utils/jsonHelpers.hpp"
#include "utils/fileUtils.hpp"

#include <iostream>
#include <fstream>

Scene::Scene(uint32_t width, uint32_t height) {
    this->rm = std::make_unique<ResourceManager>();
    Renderer* r = this->rm->renderers.create("main");
    r->init(width, height);
}

void Scene::loadSceneFromJSON(const std::string& jsonPath) {
    json j = parseJson(jsonPath);

    this->loadShaders   (j);
    this->loadMaterials (j);
    this->loadTextures  (j);
    this->loadMeshes    (j);
    this->loadModels    (j);
    this->loadLights    (j);
    this->loadPasses    (j);
}

void Scene::saveSceneToJSON(const std::string& jsonPath) {
    json j;

    std::string exeDir = getExecutableDirectory();
    std::string assetDir = exeDir + "\\assets\\";

    this->saveShaders   (j);
    this->saveMaterials (j);
    this->saveTextures  (j);
    this->saveMeshes    (j);
    this->saveModels    (j);
    this->saveLights    (j);
    this->savePasses    (j);

    std::ofstream out(jsonPath);
    if (!out.is_open()) {
        std::cerr << "Failed to open: " << jsonPath << "\n";
        return;
    }
    out << j.dump(4);
}

void Scene::createShader(const std::string& tag,
                         const std::string& vert,
                         const std::string& frag) {

    std::shared_ptr<Shader> s = this->rm->shaders.create(tag, vert, frag);
    s->vertName = vert;
    s->fragName = frag;
}

void Scene::createTexture(const std::string& tag, const std::string& path) {
    std::shared_ptr<Texture> t =this->rm->textures.create(tag, path);
    t->texSource = path;
    t->name = tag;
}

void Scene::createMaterial(const std::string& tag,
                           const float shininess,
                           std::shared_ptr<Texture> tex,
                           std::shared_ptr<Texture> spec,
                           std::shared_ptr<Texture> opacity,
                           std::shared_ptr<Texture> normal) {
    std::shared_ptr<Material> mat = this->rm->materials.create(tag);
    mat->diffuse = tex;
    mat->specular = spec;
    mat->shininess = shininess;
    mat->opacityMap = opacity;
    mat->normal = normal;
}

void Scene::createPass(const std::string& tag,
                       const std::string& shaderTag,
                       std::unordered_map<std::string, UniformValue> uniforms,
                       std::unordered_map<std::string, std::string> extraTextures,
                       const std::string& output) {

    PostProcessPass* pass = this->rm->passes.create(tag);
    pass->shaderName = shaderTag;
    pass->shader = this->rm->shaders.get(shaderTag);
    pass->uniforms = uniforms;
    pass->extraTextures = extraTextures;
    pass->saveOutputAs = output;
}

/**
* @param objFileName Leave vertices and indices blank if you are using this
*/
std::shared_ptr<Mesh> Scene::createMesh(const std::string& tag,
                                        const std::vector<float>& vertices,
                                        const std::vector<unsigned int> indices,
                                        const std::string& objFileName) {

    if (vertices.empty() != true) {
        std::shared_ptr<Mesh> m = this->rm->meshes.create(tag, vertices, 11, indices);
        m->addVertexAttribute(0, 3, GL_FLOAT, 11 * sizeof(float), (const void*)0);
        m->addVertexAttribute(1, 3, GL_FLOAT, 11 * sizeof(float), (const void*)(sizeof(float) * 3));
        m->addVertexAttribute(2, 2, GL_FLOAT, 11 * sizeof(float), (const void*)(sizeof(float) * 6));
        m->addVertexAttribute(3, 3, GL_FLOAT, 11 * sizeof(float), (const void*)(sizeof(float) * 8));

        return m;
    } else if (objFileName.empty() != true) {
        MeshStructure meshStructure = loadOBJ(objFileName);
        std::shared_ptr<Mesh> m = this->rm->meshes.create(tag, meshStructure.vertices, 11, meshStructure.indices);
        m->sourceFile = objFileName;
        m->addVertexAttribute(0, 3, GL_FLOAT, 11 * sizeof(float), (const void*)0);
        m->addVertexAttribute(1, 3, GL_FLOAT, 11 * sizeof(float), (const void*)(sizeof(float) * 3));
        m->addVertexAttribute(2, 2, GL_FLOAT, 11 * sizeof(float), (const void*)(sizeof(float) * 6));
        m->addVertexAttribute(3, 3, GL_FLOAT, 11 * sizeof(float), (const void*)(sizeof(float) * 8));

        m->submeshes = meshStructure.submeshes;

        for (auto& sm : m->submeshes) {
            m->computeBounds(sm);
        }

        for (auto& [key, mat] : meshStructure.materials) {
            this->createMaterial(key, mat->shininess, mat->diffuse, mat->specular, mat->opacityMap, mat->normal);
        }

        return m;
    } else {
        std::cerr << "Attempted to create a mesh with no vertex data or OBJ file provided for: " << tag << std::endl;
        return nullptr;
    }
}

Model* Scene::createModel(const std::string& tag,
                          const std::string& meshTag)
{
    std::shared_ptr<Mesh> m = this->rm->meshes.get(meshTag);

    Model* model = this->rm->models.create(tag, m);

    model->meshSourceName = m->sourceFile;

    return model;
}

Light* Scene::createLight(const std::string& tag) {
    return this->rm->lights.create(tag);
}

Camera* Scene::createCamera(float fovInRadians,
                            float aspectRatio,
                            float zNear,
                            float zFar) {
    Camera* cam = this->rm->cameras.create("main", fovInRadians, aspectRatio, zNear, zFar);
    return cam;
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
    return this->rm->cameras.get("main");
}

Renderer* Scene::getRenderer() {
    return this->rm->renderers.get("main");
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
        if (!m) continue;
        m->updateModelMatrix();

        glm::mat4 model = m->getModelMatrix();

        for (SubMesh& sm : m->mesh->submeshes) {
            std::shared_ptr<Material> mat = sm.material;
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

    std::sort(frame.commands.begin(), frame.commands.end(),
    [](const DrawCommand& a, const DrawCommand& b) {
        if (a.flags != b.flags)
            return a.flags < b.flags; // Opaque before Transparent
        if (a.flags & Transparent)
            return a.depth > b.depth; // Transparent: far to near
        return false;
    });


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
