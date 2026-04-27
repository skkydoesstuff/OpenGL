#include "app/app.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <memory>
#include <utils/stb_image.h>

#include "core/shader.hpp"

void App::render() {
    Model* model = scene.getModel("cube");
    std::shared_ptr<Shader> shader = scene.getShader("base");

    Renderer* r = scene.getRenderer("main");
    Camera* cam = scene.getCamera("main");

    model->updateModelMatrix();

    glm::mat4 view = cam->getView();
    glm::mat4 proj = cam->getProjection();

    r->beginScene();
    
    shader->bind();
    shader->setUniformMat4("view", view);
    shader->setUniformMat4("projection", proj);
    shader->setUniformVec3("viewPos", cam->position);

    scene.getLight("light")->upload(*shader);

    model->draw();
    
    r->endScene();

    Light::reset();
}