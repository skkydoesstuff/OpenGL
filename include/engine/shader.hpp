#pragma once

#include <unordered_map>
#include <string>

class Shader {
private:
  unsigned int vertexShaderID;
  unsigned int fragmentShaderID;
  unsigned int programID;

  std::unordered_map<std::string, unsigned int> uniformLocations;

public:
  Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath);
  ~Shader();
  void bind();
  void unbind();

private:
  unsigned int getUniformLocation(const std::string& name);

};