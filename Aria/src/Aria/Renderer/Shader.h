#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace ARIA {

struct ShaderProgramSrc {
  std::string vertex_source;
  std::string fragment_source;
};

class Shader {
public:
  Shader(const std::string &vertex_src, const std::string &fragment_src);
  Shader(const std::string &file_path);
  ~Shader();

  void bind() const;
  void unbind() const;

  void set_uniform_1i(const std::string &name, int value);

  void set_uniform_4f(const std::string &name, float v0, float v1, float v2,
                      float v3);
  void set_uniform_mat4f(const std::string &name, const glm::mat4 &matrix);
  // TODO: pull from openGL sln the uniform cache system

private:
  ShaderProgramSrc parse_shader(const std::string &file_path);
  uint32_t compile_shader(unsigned int type, const std::string &source);
  uint32_t create_shader(const std::string &vertex_shader,
                         const std::string &fragment_shader);
  const char *get_shader_type(const int shader_type) const;
  uint32_t mRendererID;

  std::unordered_map<std::string, int> mUniformLocationCache;
};
} // namespace ARIA
