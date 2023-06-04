#pragma once

#include "Aria/Renderer/Shader.h"
#include <glad/gl.h>
#include <glm/glm.hpp>

namespace aria {

class OpenGlShader : public Shader {
 public:
  OpenGlShader(const std::string &name, const std::string &vertex_src, const std::string &fragment_src);
  explicit OpenGlShader(const std::string &file_path);
  ~OpenGlShader() override;

  void Bind() const override;
  void Unbind() const override;

  const std::string &GetName() const override { return name_; }

  void SetUniform1I(const std::string &name, int value);
  void SetUniform1F(const std::string &name, float v0);
  void SetUniform1F(const std::string &name, const glm::vec1 &value);
  void SetUniform2F(const std::string &name, float v0, float v1);
  void SetUniform2F(const std::string &name, const glm::vec2 &value);
  void SetUniform3F(const std::string &name, float v0, float v1, float v2);
  void SetUniform3F(const std::string &name, const glm::vec3 &value);
  void SetUniform4F(const std::string &name, float v0, float v1, float v2, float v3);
  void SetUniform4F(const std::string &name, const glm::vec4 &value);
  void SetUniformMat3F(const std::string &name, const glm::mat3 &matrix);
  void SetUniformMat4F(const std::string &name, const glm::mat4 &matrix);

 private:
  GLenum GetShaderType(const std::string &type) const;
  std::unordered_map<GLenum, const std::string> ParseShaderFile(const std::string &file_path) const;
  uint32_t CompileShader(unsigned int type, const std::string &source) const;
  uint32_t CreateShaders(const std::unordered_map<GLenum, const std::string> &shaders) const;

  int GetUniformLocation(const std::string &name);

 private:
  uint32_t renderer_id_ = 0;
  std::unordered_map<std::string, int> uniform_location_cache_;
  std::string name_;
};
}// namespace aria
