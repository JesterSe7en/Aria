#include "Aria/Core/Base.h"
#include "ariapch.h"
#include "OpenGlShader.h"
#include "Aria/Core/Log.h"

#include <glad/gl.h>
#include <filesystem>
#include <string_view>

namespace aria {
OpenGlShader::OpenGlShader(const std::string &name, const std::string &vertex_src, const std::string &fragment_src)
    : name_(name) {
  // Pulled from Khrono's documentation
  // --------------- Vertex Shader ---------------

  // create an empty vertex shader handle
  unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);

  // Send the vertex shader source code to GL
  // Note that std::string's .c_str is NULL character terminated.
  const char *source = vertex_src.c_str();
  glad_glShaderSource(vertex_shader, 1, &source, nullptr);

  // Compile the vertex shader
  glad_glCompileShader(vertex_shader);

  int is_compiled = 0;
  glad_glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &is_compiled);
  if (is_compiled == GL_FALSE) {
    int max_length = 0;
    glad_glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &max_length);

    // The max_length includes the NULL character
    std::vector<char> info_log(max_length);
    glad_glGetShaderInfoLog(vertex_shader, max_length, &max_length, &info_log[0]);

    // We don't need the shader anymore.
    glad_glDeleteShader(vertex_shader);

    // Use the info_log as you see fit.
    ARIA_CORE_ERROR("Vertex shader compile error: {0} - Source: {1}", info_log.data(), vertex_src)
    ARIA_CORE_ASSERT(false, "Vertex shader compile error")
    return;
  }

  // --------------- Fragment Shader ---------------

  // create an empty fragment shader handle
  unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

  // Send the fragment shader source code to GL
  // Note that std::string's .c_str is NULL character terminated.
  source = fragment_src.c_str();
  glad_glShaderSource(fragment_shader, 1, &source, nullptr);

  // Compile the fragment shader
  glad_glCompileShader(fragment_shader);

  glad_glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &is_compiled);
  if (is_compiled == GL_FALSE) {
    int max_length = 0;
    glad_glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &max_length);

    // The max_length includes the NULL character
    std::vector<char> info_log(max_length);
    glad_glGetShaderInfoLog(fragment_shader, max_length, &max_length, &info_log[0]);

    // We don't need the shader anymore.
    glad_glDeleteShader(fragment_shader);
    // Either of them. Don't leak shaders.
    glad_glDeleteShader(vertex_shader);

    // Use the info_log as you see fit.
    ARIA_CORE_ERROR("Fragment shader compile error: {0} - Source: {1}", info_log.data(), fragment_src)
    ARIA_CORE_ASSERT(false, "Fragment shader compile error")
    return;
  }

  // --------------- Creating Shader Program ---------------

  // Vertex and fragment shaders are successfully compiled.
  // Now time to link them together into a program.
  // get a program object.
  renderer_id_ = glCreateProgram();

  // Attach our shaders to our program
  glad_glAttachShader(renderer_id_, vertex_shader);
  glad_glAttachShader(renderer_id_, fragment_shader);

  // Link our program
  glad_glLinkProgram(renderer_id_);

  // Note the different functions here: glGetProgram* instead of glGetShader*.
  int is_linked = 0;
  glad_glGetProgramiv(renderer_id_, GL_LINK_STATUS, &is_linked);
  if (is_linked == GL_FALSE) {
    int max_length = 0;
    glad_glGetProgramiv(renderer_id_, GL_INFO_LOG_LENGTH, &max_length);

    // The max_length includes the NULL character
    std::vector<char> infoLog(max_length);
    glad_glGetProgramInfoLog(renderer_id_, max_length, &max_length, &infoLog[0]);

    // We don't need the program anymore.
    glad_glDeleteProgram(renderer_id_);
    // Don't leak shaders either.
    glad_glDeleteShader(vertex_shader);
    glad_glDeleteShader(fragment_shader);

    // Use the info_log as you see fit.
    ARIA_CORE_ERROR("Shader linking error: {0}", infoLog.data())
    ARIA_CORE_ASSERT(false, "Shader linking error")
    // In this simple program, we'll just leave
    return;
  }

  // Always detach shaders after a successful link.
  glad_glDetachShader(renderer_id_, vertex_shader);
  glad_glDetachShader(renderer_id_, fragment_shader);
}

// maybe convert the params to std::filepath
OpenGlShader::OpenGlShader(const std::string &file_path) {
  if (!std::filesystem::exists(file_path)) {
    ARIA_CORE_WARN("Cannot find shader file: {0}", file_path)
    return;
  }
  auto shaders = ParseShaderFile(file_path);
  renderer_id_ = CreateShaders(shaders);

  // Gets file name without file extension
  name_ = std::filesystem::path(file_path).filename().stem().string();
}

OpenGlShader::~OpenGlShader() { glad_glDeleteProgram(renderer_id_); }

void OpenGlShader::Bind() const { glad_glUseProgram(renderer_id_); }

void OpenGlShader::Unbind() const { glad_glUseProgram(0); }

void OpenGlShader::SetUniform1I(const std::string &name, int value) {
  glad_glUniform1i(GetUniformLocation(name), value);
}

void OpenGlShader::SetUniform1F(const std::string &name, float v0) {
  glad_glUniform1f(GetUniformLocation(name), v0);
}

void OpenGlShader::SetUniform1F(const std::string &name, const glm::vec1 &value) {
  glad_glUniform1f(GetUniformLocation(name), value.x);
}

void OpenGlShader::SetUniform2F(const std::string &name, float v0, float v1) {
  glad_glUniform2f(GetUniformLocation(name), v0, v1);
}

void OpenGlShader::SetUniform2F(const std::string &name, const glm::vec2 &value) {
  glad_glUniform2f(GetUniformLocation(name), value.x, value.y);
}

void OpenGlShader::SetUniform3F(const std::string &name, float v0, float v1, float v2) {
  glad_glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void OpenGlShader::SetUniform3F(const std::string &name, const glm::vec3 &value) {
  glad_glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void OpenGlShader::SetUniform4F(const std::string &name, float v0, float v1, float v2, float v3) {
  glad_glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void OpenGlShader::SetUniform4F(const std::string &name, const glm::vec4 &value) {
  glad_glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void OpenGlShader::SetUniformMat3F(const std::string &name, const glm::mat3 &matrix) {
  glad_glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

void OpenGlShader::SetUniformMat4F(const std::string &name, const glm::mat4 &matrix) {
  glad_glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

#pragma region Shader Compilation

GLenum OpenGlShader::GetShaderType(const std::string_view type) const {
  // All possible types in OpenGl
  // GL_COMPUTE_SHADER, GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER, or
  // GL_FRAGMENT_SHADER.

  if (type == "vertex") {
    return GL_VERTEX_SHADER;
  } else if (type == "fragment" || type == "pixel") {
    return GL_FRAGMENT_SHADER;
  }
  ARIA_CORE_ASSERT(false, "Unknown shader type")
  return 0;
}

std::unordered_map<GLenum, const std::string> OpenGlShader::ParseShaderFile(const std::string &file_path) const {
  std::unordered_map<GLenum, const std::string> shaders;
  std::ifstream stream(file_path);

  std::string line;
  std::stringstream ss[1];
  GLenum shader_type = 0;

  // There's probably a better, more robust way to do this...
  const char *type_token = "#type";
  size_t size = strlen(type_token);
  while (getline(stream, line)) {
    if (line.find("#type") != std::string::npos) {
      size_t eol = line.find_first_of("\r\n");
      std::string type = line.substr(size + 1, eol);
      if (shader_type == 0) {
        shader_type = GetShaderType(type);
      } else {
        shaders.emplace(shader_type, ss[0].str());
        shader_type = 0;
        ss[0].str("");

        shader_type = GetShaderType(type);
      }
    } else {
      ss[0] << line << '\n';
    }
  }

  if (shader_type != 0) {
    shaders.emplace(shader_type, ss[0].str());
  }

  ARIA_CORE_ASSERT(shaders.size() != 0, "Parser did not find any valid shader code")
  return shaders;
}

uint32_t OpenGlShader::CompileShader(unsigned int type, const std::string &source) const {
  // create an empty vertex shader handle
  int shader_type;
  unsigned int id = glad_glCreateShader(type);
  glad_glGetShaderiv(id, GL_SHADER_TYPE, &shader_type);

  // Send the vertex shader source code to GL
  // Note that std::string's .c_str is NULL character terminated.
  const char *src = source.c_str();
  glad_glShaderSource(id, 1, &src, nullptr);

  // Compile the vertex shader
  glad_glCompileShader(id);

  int is_compiled = 0;
  glad_glGetShaderiv(id, GL_COMPILE_STATUS, &is_compiled);
  if (is_compiled == GL_FALSE) {
    int max_length = 0;
    glad_glGetShaderiv(id, GL_INFO_LOG_LENGTH, &max_length);

    // The max_length includes the NULL character
    std::vector<char> info_log(max_length);
    glad_glGetShaderInfoLog(id, max_length, &max_length, &info_log[0]);

    // We don't need the shader anymore.
    glad_glDeleteShader(id);

    // Use the info_log as you see fit.
    ARIA_CORE_ERROR("Shader compile error: {0} - Type: {1} - Source: {2}", info_log.data(), type, src)
    ARIA_CORE_ASSERT(false, "Shader compile error")
    return 0;
  }
  return id;
}

uint32_t OpenGlShader::CreateShaders(const std::unordered_map<GLenum, const std::string> &shaders) const {
  if (shaders.empty()) {
    return 0;
  }

  uint32_t id = glad_glCreateProgram();
  ARIA_CORE_ASSERT(shaders.size() <= 2, "Only supports two shaders at the moment")
  std::array<GLenum, 2> shader_ids;
  int idx_shader = 0;
  for (const auto &[kType, kSource] : shaders) {
    uint32_t compiled = CompileShader(kType, kSource);
    shader_ids[idx_shader] = compiled;
    idx_shader++;
    glad_glAttachShader(id, compiled);
  }

  // Link our program
  glad_glLinkProgram(id);

  // Note the different functions here: glGetProgram* instead of glGetShader*.
  int is_linked = 0;
  glad_glGetProgramiv(id, GL_LINK_STATUS, &is_linked);
  if (is_linked == GL_FALSE) {
    int max_length = 0;
    glad_glGetProgramiv(id, GL_INFO_LOG_LENGTH, &max_length);

    // The max_length includes the NULL character
    std::vector<char> info_log(max_length);
    glad_glGetProgramInfoLog(id, max_length, &max_length, &info_log[0]);

    // We don't need the program anymore.
    glad_glDeleteProgram(id);
    // Don't leak shaders either.
    for (auto const &kShaderId : shader_ids) {
      glad_glDeleteShader(kShaderId);
    }

    // Use the info_log as you see fit.
    ARIA_CORE_ERROR("Shader linking error: {0}", info_log.data())
    ARIA_CORE_ASSERT(false, "Shader linking error")
    // In this simple program, we'll just leave
    return 0;
  }

  for (auto const &kShaderId : shader_ids) {
    glad_glDetachShader(id, kShaderId);
  }

  return id;
}

#pragma endregion

int OpenGlShader::GetUniformLocation(const std::string &name) {
  if (uniform_location_cache_.find(name) != uniform_location_cache_.end()) return uniform_location_cache_[name];

  int location = glad_glGetUniformLocation(renderer_id_, name.c_str());
  if (location == -1) ARIA_CORE_WARN("Warning: Uniform '{0}' does not exist", name)

  uniform_location_cache_[name] = location;
  return location;
}

}  // namespace ARIA
