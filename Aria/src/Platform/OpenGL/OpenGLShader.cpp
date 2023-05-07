#include "Aria/Core/Base.h"
#include "Aria/Renderer/Shader.h"
#include "ariapch.h"
#include "OpenGLShader.h"
#include "Aria/Core/Log.h"

#include <glad/gl.h>
#include <stdint.h>
#include <filesystem>
#include <string_view>

namespace ARIA {
OpenGLShader::OpenGLShader(const std::string &vertex_src, const std::string &fragment_src) {
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
  mRendererID = glCreateProgram();

  // Attach our shaders to our program
  glad_glAttachShader(mRendererID, vertex_shader);
  glad_glAttachShader(mRendererID, fragment_shader);

  // Link our program
  glad_glLinkProgram(mRendererID);

  // Note the different functions here: glGetProgram* instead of glGetShader*.
  int is_linked = 0;
  glad_glGetProgramiv(mRendererID, GL_LINK_STATUS, &is_linked);
  if (is_linked == GL_FALSE) {
    int max_length = 0;
    glad_glGetProgramiv(mRendererID, GL_INFO_LOG_LENGTH, &max_length);

    // The max_length includes the NULL character
    std::vector<char> infoLog(max_length);
    glad_glGetProgramInfoLog(mRendererID, max_length, &max_length, &infoLog[0]);

    // We don't need the program anymore.
    glad_glDeleteProgram(mRendererID);
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
  glad_glDetachShader(mRendererID, vertex_shader);
  glad_glDetachShader(mRendererID, fragment_shader);
}

OpenGLShader::OpenGLShader(const std::string &file_path) {
  if (!std::filesystem::exists(file_path)) {
    ARIA_CORE_WARN("Cannot find shader file: {0}", file_path)
    return;
  }
  auto shaders = parse_shader_file(file_path);
  mRendererID = create_shaders(shaders);
}

OpenGLShader::~OpenGLShader() { glad_glDeleteProgram(mRendererID); }

void OpenGLShader::bind() const { glad_glUseProgram(mRendererID); }

void OpenGLShader::unbind() const { glad_glUseProgram(0); }

void OpenGLShader::set_uniform_1i(const std::string &name, int value) {
  glad_glUniform1i(get_uniform_location(name), value);
}

void OpenGLShader::set_uniform_1f(const std::string &name, float v0) {
  glad_glUniform1f(get_uniform_location(name), v0);
}

void OpenGLShader::set_uniform_1f(const std::string &name, const glm::vec1 &value) {
  glad_glUniform1f(get_uniform_location(name), value.x);
}

void OpenGLShader::set_uniform_2f(const std::string &name, float v0, float v1) {
  glad_glUniform2f(get_uniform_location(name), v0, v1);
}

void OpenGLShader::set_uniform_2f(const std::string &name, const glm::vec2 &value) {
  glad_glUniform2f(get_uniform_location(name), value.x, value.y);
}

void OpenGLShader::set_uniform_3f(const std::string &name, float v0, float v1, float v2) {
  glad_glUniform3f(get_uniform_location(name), v0, v1, v2);
}

void OpenGLShader::set_uniform_3f(const std::string &name, const glm::vec3 &value) {
  glad_glUniform3f(get_uniform_location(name), value.x, value.y, value.z);
}

void OpenGLShader::set_uniform_4f(const std::string &name, float v0, float v1, float v2, float v3) {
  glad_glUniform4f(get_uniform_location(name), v0, v1, v2, v3);
}

void OpenGLShader::set_uniform_4f(const std::string &name, const glm::vec4 &value) {
  glad_glUniform4f(get_uniform_location(name), value.x, value.y, value.z, value.w);
}

void OpenGLShader::set_uniform_mat3f(const std::string &name, const glm::mat3 &matrix) {
  glad_glUniformMatrix3fv(get_uniform_location(name), 1, GL_FALSE, &matrix[0][0]);
}

void OpenGLShader::set_uniform_mat4f(const std::string &name, const glm::mat4 &matrix) {
  glad_glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, &matrix[0][0]);
}

#pragma region Shader Compilation

GLenum OpenGLShader::get_shader_type(const std::string_view line) const {
  // All possible types in OpenGl
  // GL_COMPUTE_SHADER, GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER, or
  // GL_FRAGMENT_SHADER.

  if (line == "vertex") {
    return GL_VERTEX_SHADER;
  } else if (line == "fragment" || line == "pixel") {
    return GL_FRAGMENT_SHADER;
  }
  ARIA_CORE_ASSERT(false, "Unknown shader type")
  return 0;
}

std::unordered_map<GLenum, const std::string> OpenGLShader::parse_shader_file(const std::string &file_path) const {
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
        shader_type = get_shader_type(type);
      } else {
        shaders.emplace(shader_type, ss[0].str());
        shader_type = 0;
        ss[0].str("");

        shader_type = get_shader_type(type);
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

uint32_t OpenGLShader::compile_shader(unsigned int type, const std::string &source) const {
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
    std::vector<char> infoLog(max_length);
    glad_glGetShaderInfoLog(id, max_length, &max_length, &infoLog[0]);

    // We don't need the shader anymore.
    glad_glDeleteShader(id);

    // Use the info_log as you see fit.
    ARIA_CORE_ERROR("Shader compile error: {0} - Type: {1} - Source: {2}", infoLog.data(), type, src)
    ARIA_CORE_ASSERT(false, "Shader compile error")
    return 0;
  }
  return id;
}

uint32_t OpenGLShader::create_shaders(const std::unordered_map<GLenum, const std::string> &shaders) const {
  if (shaders.empty()) {
    return 0;
  }

  uint32_t id = glad_glCreateProgram();
  ARIA_CORE_ASSERT(shaders.size() <= 2, "Only supports two shaders at the moment")
  std::array<GLenum, 2> shader_ids;
  int idx_shader = 0;
  for (const auto &[type, source] : shaders) {
    uint32_t compiled = compile_shader(type, source);
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
    std::vector<char> infoLog(max_length);
    glad_glGetProgramInfoLog(id, max_length, &max_length, &infoLog[0]);

    // We don't need the program anymore.
    glad_glDeleteProgram(id);
    // Don't leak shaders either.
    for (auto const &shader_id : shader_ids) {
      glad_glDeleteShader(shader_id);
    }

    // Use the info_log as you see fit.
    ARIA_CORE_ERROR("Shader linking error: {0}", infoLog.data())
    ARIA_CORE_ASSERT(false, "Shader linking error")
    // In this simple program, we'll just leave
    return 0;
  }

  for (auto const &shader_id : shader_ids) {
    glad_glDetachShader(id, shader_id);
  }

  return id;
}

#pragma endregion

int OpenGLShader::get_uniform_location(const std::string &name) {
  if (mUniformLocationCache.find(name) != mUniformLocationCache.end()) return mUniformLocationCache[name];

  int location = glad_glGetUniformLocation(mRendererID, name.c_str());
  if (location == -1) ARIA_CORE_WARN("Warning: Uniform '{0}' does not exist", name)

  mUniformLocationCache[name] = location;
  return location;
}

}  // namespace ARIA
