#pragma once

#include <string>

namespace Aria {

  struct ShaderProgramSrc {
  std::string vertex_source;
  std::string fragment_source;
};

class Shader {
 public:
  Shader(const std::string& vertex_src, const std::string& fragment_src);
  Shader(const std::string& file_path);
  ~Shader();

  void Bind() const;
  void Unbind() const;

  // TODO: pull from openGL sln the uniform cache system

 private:
  ShaderProgramSrc ParseShader(const std::string& file_path);
  uint32_t CompileShader(unsigned int type, const std::string& source);
  uint32_t CreateShader(const std::string& vertex_shader,
                        const std::string& fragment_shader);
  const char* GetShaderType(const int shader_type) const;
  uint32_t renderer_id_;
};
}  // namespace Aria