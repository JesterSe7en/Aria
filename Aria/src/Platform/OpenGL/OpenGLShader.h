#pragma once

#include "Aria/Renderer/Shader.h"
#include <glad/gl.h>
#include <glm/glm.hpp>

namespace ARIA {

class OpenGLShader : public Shader {
 public:
  OpenGLShader(const std::string &name, const std::string &vertex_src, const std::string &fragment_src);
  explicit OpenGLShader(const std::string &file_path);
  ~OpenGLShader() override;

  void bind() const override;
  void unbind() const override;

  const std::string get_name() const override { return mName; }

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
  GLenum get_shader_type(const std::string_view type) const;
  std::unordered_map<GLenum, const std::string> parse_shader_file(const std::string &file_path) const;
  uint32_t compile_shader(unsigned int type, const std::string &source) const;
  uint32_t create_shaders(const std::unordered_map<GLenum, const std::string> &shaders) const;

  int get_uniform_location(const std::string &name);

 private:
  uint32_t mRendererID = 0;
  std::unordered_map<std::string, int> mUniformLocationCache;
  std::string mName;
};
}  // namespace ARIA
