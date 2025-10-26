#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shader.hpp"
#include "arrayBuffer.hpp"
#include "vertexArray.hpp"

#include "utils.hpp"

#include <iostream>

#define WIDTH 800
#define HEIGHT 600
#define TITLE "Window"

#define OPENGL_VERSION_MAJOR 3
#define OPENGL_VERSION_MINOR 3

int main() {
	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	std::cout << "OpenGL Version: " << OPENGL_VERSION_MAJOR << '.' << OPENGL_VERSION_MINOR << std::endl;

	GLFWwindow* window;
	window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);

	constexpr float vertices[] = {
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 1.0f
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

		VAO.setVertexAttrib(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void*>(0));
		VAO.setVertexAttrib(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3*sizeof(float)));

		std::string vertexShaderSource = readFileIntoString("../assets/shaders/shader.vert");
		std::string fragmentShaderSource = readFileIntoString("../assets/shaders/shader.frag"); 

		Shader shaderObj(vertexShaderSource, fragmentShaderSource);
		shaderObj.Bind();

		while (!glfwWindowShouldClose(window)) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);		
		
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}
