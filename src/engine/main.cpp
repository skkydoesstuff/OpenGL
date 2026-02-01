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

    float vertices[9] = {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    Shader shader("shader.vert", "shader.frag");
    shader.Bind();

    Mesh triangle(vertices, 3, 3, NULL, 0);
    Entity triangle_e(&triangle, &shader);

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

        shader.setMat4f(view, "view");
        shader.setMat4f(projection, "projection");

        triangle_e.transform.rotation.y += 0.1f;
        triangle_e.draw();

        glBindVertexArray(triangle.VAO);
        glDrawArrays(GL_TRIANGLES, 0, triangle.vertexCount);

        SDL_GL_SwapWindow(state.window);
    }

    SDL_GL_DestroyContext(state.context);
    SDL_DestroyWindow(state.window);
}