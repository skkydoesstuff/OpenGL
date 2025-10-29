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

#include <iostream>

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
		VAO.setVertexAttrib(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6*sizeof(float)));
    
    std::string exeDir = getExecutableDir();

    std::string vertFilePath = exeDir + "/assets/shaders/shader.vert";
    std::string fragFilePath = exeDir + "/assets/shaders/shader.frag";

		std::string vertexShaderSource = readFileIntoString(vertFilePath);
		std::string fragmentShaderSource = readFileIntoString(fragFilePath); 

		Shader shaderObj(vertexShaderSource, fragmentShaderSource);
		shaderObj.Bind();

    Texture containerTexture(exeDir + "/assets/textures/container.jpg");

		while (!windowObj.GetWindowShouldClose()) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

      containerTexture.bind(0);
			EBO.Draw(GL_TRIANGLES, 6, GL_UNSIGNED_INT);
      containerTexture.unbind();

			windowObj.SwapBuffers();
			glfwPollEvents();
		}
	}
}
