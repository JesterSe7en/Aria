#pragma once

#include "Aria/Renderer/Shader.h"
#include <glm/glm.hpp>

namespace ARIA {

struct ShaderProgramSrc {
  std::string vertex_source;
  std::string fragment_source;
};

class OpenGLShader : public Shader {
 public:
  OpenGLShader(const std::string &vertex_src, const std::string &fragment_src);
  OpenGLShader(const std::string &file_path);
  virtual ~OpenGLShader();

  void bind() const override;
  void unbind() const override;

  void set_uniform_1i(const std::string &name, int value);
  void set_uniform_1f(const std::string &name, float v0);
  void set_uniform_1f(const std::string &name, const glm::vec1 &value);
  void set_uniform_2f(const std::string &name, float v0, float v1);
  void set_uniform_2f(const std::string &name, const glm::vec2 &value);
  void set_uniform_3f(const std::string &name, float v0, float v1, float v2);
  void set_uniform_3f(const std::string &name, const glm::vec3 &value);
  void set_uniform_4f(const std::string &name, float v0, float v1, float v2, float v3);
  void set_uniform_4f(const std::string &name, const glm::vec4 &value);
  void set_uniform_mat3f(const std::string &name, const glm::mat3 &matrix);
  void set_uniform_mat4f(const std::string &name, const glm::mat4 &matrix);

 private:
  ShaderProgramSrc parse_shader(const std::string &file_path);
  uint32_t compile_shader(unsigned int type, const std::string &source);
  uint32_t create_shader(const std::string &vertex_shader, const std::string &fragment_shader);
  const char *get_shader_type(const int shader_type) const;
  int get_uniform_location(const std::string &name);

 private:
  uint32_t mRendererID;
  std::unordered_map<std::string, int> mUniformLocationCache;
};
}  // namespace ARIA
