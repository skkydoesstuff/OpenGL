#include <glad/glad.h>

#include <shader.hpp>

#include <string>
#include <iostream>

Shader::Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
	program = createProgram(vertexShaderSource, fragmentShaderSource);	
}

Shader::~Shader() {
	glDeleteProgram(program);
}

void Shader::Bind() {
	glUseProgram(program);
}

void Shader::Unbind() {
	glUseProgram(0);
}

void Shader::SetInt(const std::string& name, int value) {
  unsigned int loc = getUniformLocation(program, name);
  glUniform1i(loc, value);
}

unsigned int Shader::createShader(const std::string& shaderSource, unsigned int type) {
	unsigned int shader = glCreateShader(type);
	const char* shaderSrc = shaderSource.c_str();
	glShaderSource(shader, 1, &shaderSrc, nullptr);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
    	glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    	std::cerr << "Vertex shader error:\n" << infoLog << std::endl;
	}	

	return shader;
}

unsigned int Shader::createProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
	unsigned int vertexShader = createShader(vertexShaderSource, GL_VERTEX_SHADER);
	unsigned int fragmentShader = createShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
	
	unsigned int program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

unsigned int Shader::getUniformLocation(unsigned int programID, const std::string& name) {
  return glGetUniformLocation(programID, name.c_str()); 
}