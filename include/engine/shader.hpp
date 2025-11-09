#pragma once

#include <glm/glm.hpp>

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

  void setInt(std::string name, int value);
  void setMat4(std::string name, glm::mat4 mat);

private:
  unsigned int getUniformLocation(const std::string& name);

};