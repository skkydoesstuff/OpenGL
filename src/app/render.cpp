#include "app/app.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <utils/stb_image.h>

#include "core/shader.hpp"

void App::render() {
    std::shared_ptr<Model> model = this->rm.models.get("cube");
    std::shared_ptr<Shader> shader = this->rm.shaders.get("base");
    
    std::shared_ptr<Renderer> r = this->rm.renderers.get("main");

    model->updateModelMatrix();

    glm::mat4 view = this->cam->getView();
    glm::mat4 proj = this->cam->getProjection();
    
    r->beginScene();
    
    shader->bind();
    shader->setUniformMat4("view", view);       
    shader->setUniformMat4("projection", proj);
    shader->setUniformVec3("viewPos", cam->position);
    this->rm.lights.get("light")->upload(*shader);
    this->rm.materials.get("container")->bind(*shader);

    model->draw();

    r->endScene();
}