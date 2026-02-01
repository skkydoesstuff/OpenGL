#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <string>
#include <unordered_map>

#include "error_handling.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include "entity.hpp"
#include "material.hpp"
#include "texture.hpp"

#define WIDTH 800
#define HEIGHT 600
#define TITLE "title"
#define WINDOW_FLAGS (SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_OPENGL)

typedef struct engineState {
    SDL_Window* window;
    SDL_GLContext context;
} engineState;

engineState init() {
    engineState s = {0};

    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    s.window = SDL_CreateWindow(TITLE, WIDTH, HEIGHT, WINDOW_FLAGS);
    
    s.context = SDL_GL_CreateContext(s.window);
    SDL_GL_MakeCurrent(s.window, s.context);

    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        exception("failed to load GLAD!");
    }

    glViewport(0,0,WIDTH,HEIGHT);
    glEnable(GL_DEPTH_TEST);

    return s;
}

int main() {
    engineState state = init();

    float vertices[] = {
        // positions        // uvs
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,  // top-left
        0.5f,  0.5f, 0.0f,  1.0f, 1.0f,  // top-right
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  // bottom-right
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f   // bottom-left
    };

    unsigned int indices[] = {
        0, 1, 2,  // first triangle (top-left, top-right, bottom-right)
        2, 3, 0   // second triangle (bottom-right, bottom-left, top-left)
    };

    Shader shader("shader.vert", "shader.frag");
    shader.Bind();

    Mesh square(vertices, 4, 5, indices, 6);
    unsigned int tex = LoadTexture("brick.png");
    Material mat(&shader, 0);
    Entity square_e(&square, &mat);
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    SDL_Event e;

    bool isRunning = true;

    while (isRunning) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) isRunning = false;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat.shader->Bind();
        mat.shader->setMat4f(view, "view");
        mat.shader->setMat4f(projection, "projection");

        square_e.transform.rotation.y += 0.1f;
        square_e.Render();

        SDL_GL_SwapWindow(state.window);
    }

    SDL_GL_DestroyContext(state.context);
    SDL_DestroyWindow(state.window);
}