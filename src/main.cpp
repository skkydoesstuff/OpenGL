#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stb_image.h>

#include "window.hpp"
#include "shader.hpp"
#include "arrayBuffer.hpp"
#include "vertexArray.hpp"
#include "texture.hpp"

#include "defines.hpp"
#include "utils.hpp"

int main() {
	Window windowObj(WIDTH, HEIGHT, TITLE);

	constexpr float vertices[] = {
    // coords             // color            // texcoords
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 1.0f,   0.0f, 1.0f, // top left
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 1.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 1.0f,   0.0f, 0.0f // bottom left
	};

	constexpr unsigned int indices[] = {
		0, 2, 3,
		0, 1, 2
	};

	{
		VertexArray VAO;
		VAO.Bind();

		ArrayBuffer VBO(GL_ARRAY_BUFFER, vertices, sizeof(vertices));
		VBO.Bind();

		ArrayBuffer EBO(GL_ELEMENT_ARRAY_BUFFER, indices, sizeof(indices));
		EBO.Bind();

		VAO.setVertexAttrib(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void*>(0));
		VAO.setVertexAttrib(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3*sizeof(float)));
		VAO.setVertexAttrib(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6*sizeof(float)));
    
    std::string exeDir = getExecutableDir();

		std::string vertexShaderSource = readFileIntoString(exeDir + "/assets/shaders/shader.vert");
		std::string fragmentShaderSource = readFileIntoString(exeDir + "/assets/shaders/shader.frag"); 

		Shader shaderObj(vertexShaderSource, fragmentShaderSource);
		shaderObj.Bind();

    shaderObj.SetInt("texture1", 0);
    shaderObj.SetInt("texture2", 1);

    Texture texture1(exeDir + "/assets/textures/container.jpg", shaderObj, 0);
    Texture texture2(exeDir + "/assets/textures/awesome.png", shaderObj, 1);

		while (!windowObj.GetWindowShouldClose()) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

      texture1.bind();
      texture2.bind();
			EBO.Draw(GL_TRIANGLES, 6, GL_UNSIGNED_INT);
      texture1.unbind();
      texture2.unbind();

			windowObj.SwapBuffers();
			glfwPollEvents();
		}
	}
}
