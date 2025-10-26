#pragma once

#include <string>

class Shader {
private:
	unsigned int program;
	
public:
	Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
	~Shader();

	void Bind();
	void Unbind();

private:
	static unsigned int createShader(const std::string& shaderSource, unsigned int type);
	static unsigned int createProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
};
