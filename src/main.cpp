#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stb_image.h>

#include "window.hpp"
#include "shader.hpp"
#include "arrayBuffer.hpp"
#include "vertexArray.hpp"

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

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;

    std::string container_path = exeDir + "/assets/textures/container.jpg";
    unsigned char *data = stbi_load(container_path.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      std::cout << "Failed to load texture!" << std::endl;
    }
    stbi_image_free(data);

		while (!windowObj.GetWindowShouldClose()) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

      glBindTexture(GL_TEXTURE_2D, texture);
			EBO.Draw(GL_TRIANGLES, 6, GL_UNSIGNED_INT);
		
			windowObj.SwapBuffers();
			glfwPollEvents();
		}
	}
}
