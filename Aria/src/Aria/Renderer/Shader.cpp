#include "ariapch.h"
#include "Shader.h"
#include "Aria/Log.h"

#include <filesystem>
#include <glad/gl.h>

namespace Aria {

Shader::Shader(const std::string &vertex_src, const std::string &fragment_src)
    : renderer_id_(0) {
  // Pulled from Khrono's documentation
  // --------------- Vertex Shader ---------------

  // Create an empty vertex shader handle
  unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);

  // Send the vertex shader source code to GL
  // Note that std::string's .c_str is NULL character terminated.
  const char *source = vertex_src.c_str();
  glShaderSource(vertex_shader, 1, &source, 0);

  // Compile the vertex shader
  glCompileShader(vertex_shader);

  int is_compiled = 0;
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &is_compiled);
  if (is_compiled == GL_FALSE) {
    int max_length = 0;
    glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &max_length);

    // The max_length includes the NULL character
    std::vector<char> infoLog(max_length);
    glGetShaderInfoLog(vertex_shader, max_length, &max_length, &infoLog[0]);

    // We don't need the shader anymore.
    glDeleteShader(vertex_shader);

    // Use the infoLog as you see fit.
    ARIA_CORE_ERROR("Vertex shader compile error: {0} - Source: {1}",
                    infoLog.data(), vertex_src);
    ARIA_CORE_ASSERT(false);
    return;
  }

  // --------------- Fragment Shader ---------------

  // Create an empty fragment shader handle
  unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

  // Send the fragment shader source code to GL
  // Note that std::string's .c_str is NULL character terminated.
  source = (const char *)fragment_src.c_str();
  glShaderSource(fragment_shader, 1, &source, 0);

  // Compile the fragment shader
  glCompileShader(fragment_shader);

  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &is_compiled);
  if (is_compiled == GL_FALSE) {
    int max_length = 0;
    glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &max_length);

    // The max_length includes the NULL character
    std::vector<char> infoLog(max_length);
    glGetShaderInfoLog(fragment_shader, max_length, &max_length, &infoLog[0]);

    // We don't need the shader anymore.
    glDeleteShader(fragment_shader);
    // Either of them. Don't leak shaders.
    glDeleteShader(vertex_shader);

    // Use the infoLog as you see fit.
    ARIA_CORE_ERROR("Fragment shader compile error: {0} - Source: {1}",
                    infoLog.data(), fragment_src);
    ARIA_CORE_ASSERT(false);
    return;
  }

  // --------------- Creating Shader Program ---------------

  // Vertex and fragment shaders are successfully compiled.
  // Now time to link them together into a program.
  // Get a program object.
  renderer_id_ = glCreateProgram();

  // Attach our shaders to our program
  glAttachShader(renderer_id_, vertex_shader);
  glAttachShader(renderer_id_, fragment_shader);

  // Link our program
  glLinkProgram(renderer_id_);

  // Note the different functions here: glGetProgram* instead of glGetShader*.
  int is_linked = 0;
  glGetProgramiv(renderer_id_, GL_LINK_STATUS, (int *)&is_linked);
  if (is_linked == GL_FALSE) {
    int max_length = 0;
    glGetProgramiv(renderer_id_, GL_INFO_LOG_LENGTH, &max_length);

    // The max_length includes the NULL character
    std::vector<char> infoLog(max_length);
    glGetProgramInfoLog(renderer_id_, max_length, &max_length, &infoLog[0]);

    // We don't need the program anymore.
    glDeleteProgram(renderer_id_);
    // Don't leak shaders either.
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // Use the infoLog as you see fit.
    ARIA_CORE_ERROR("Shader linking error: {0}", infoLog.data());
    ARIA_CORE_ASSERT(false);
    // In this simple program, we'll just leave
    return;
  }

  // Always detach shaders after a successful link.
  glDetachShader(renderer_id_, vertex_shader);
  glDetachShader(renderer_id_, fragment_shader);
}

Shader::Shader(const std::string &file_path) : renderer_id_(0) {

  if (!std::filesystem::exists(file_path)) {
    ARIA_CORE_WARN("Cannot find shader file: {0}", file_path);
  };

  ShaderProgramSrc src = ParseShader(file_path);
  renderer_id_ = CreateShader(src.vertex_source, src.fragment_source);
}

Shader::~Shader() { glDeleteProgram(renderer_id_); }

void Shader::Bind() const { glUseProgram(renderer_id_); }

void Shader::Unbind() const { glUseProgram(0); }

// TODO: only supports vertex and frag shaders
ShaderProgramSrc Shader::ParseShader(const std::string &file_path) {
  // we need to parse and find our special syntax e.g. (#shader vertex)

  std::ifstream stream(file_path);

  enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

  std::string line;
  std::stringstream ss[2];
  ShaderType type = ShaderType::NONE;
  while (getline(stream, line)) {
    if (line.find("#shader") != std::string::npos) {
      if (line.find("vertex") != std::string::npos) {
        // set mode to vertex
        type = ShaderType::VERTEX;
      } else if (line.find("fragment") != std::string::npos) {
        // set mode to fragment
        type = ShaderType::FRAGMENT;
      }
    } else {
      ss[(int)type] << line << '\n';
    }
  }
  return {ss[0].str(), ss[1].str()};
}

uint32_t Shader::CompileShader(unsigned int type, const std::string &source) {
  // Create an empty vertex shader handle
  int shader_type;
  unsigned int id = glCreateShader(type);
  glGetShaderiv(id, GL_SHADER_TYPE, &shader_type);
  

  // Send the vertex shader source code to GL
  // Note that std::string's .c_str is NULL character terminated.
  const char *src = source.c_str();
  glShaderSource(id, 1, &src, 0);

  // Compile the vertex shader
  glCompileShader(id);

  int is_compiled = 0;
  glGetShaderiv(id, GL_COMPILE_STATUS, &is_compiled);
  if (is_compiled == GL_FALSE) {
    int max_length = 0;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &max_length);

    // The max_length includes the NULL character
    std::vector<char> infoLog(max_length);
    glGetShaderInfoLog(id, max_length, &max_length, &infoLog[0]);

    // We don't need the shader anymore.
    glDeleteShader(id);

    // Use the infoLog as you see fit.
    ARIA_CORE_ERROR("Shader compile error: {0} - Type: {1} - Source: {2}",
                    infoLog.data(), type, src);
    ARIA_CORE_ASSERT(false);
    return 0;
  }
  return id;
}

// TODO: accept other kinds of shaders, prob pass a vector
uint32_t Shader::CreateShader(const std::string &vertex_shader,
                              const std::string &fragment_shader) {
  uint32_t id = glCreateProgram();
  uint32_t vs = CompileShader(GL_VERTEX_SHADER, vertex_shader);
  uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, fragment_shader);

  // Attach our shaders to our program
  glAttachShader(id, vs);
  glAttachShader(id, fs);

  // Link our program
  glLinkProgram(id);

  // Note the different functions here: glGetProgram* instead of glGetShader*.
  int is_linked = 0;
  glGetProgramiv(id, GL_LINK_STATUS, (int *)&is_linked);
  if (is_linked == GL_FALSE) {
    int max_length = 0;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &max_length);

    // The max_length includes the NULL character
    std::vector<char> infoLog(max_length);
    glGetProgramInfoLog(id, max_length, &max_length, &infoLog[0]);

    // We don't need the program anymore.
    glDeleteProgram(id);
    // Don't leak shaders either.
    glDeleteShader(vs);
    glDeleteShader(fs);

    // Use the infoLog as you see fit.
    ARIA_CORE_ERROR("Shader linking error: {0}", infoLog.data());
    ARIA_CORE_ASSERT(false);
    // In this simple program, we'll just leave
    return 0;
  }

  // Always detach shaders after a successful link.
  glDetachShader(id, vs);
  glDetachShader(id, fs);

  return id;
}

const char *Shader::GetShaderType(const int shader_type) const {
  const char *type;
  switch (shader_type) {
    case GL_VERTEX_SHADER:
      type = "Vertex Shader";
      break;
    case GL_FRAGMENT_SHADER:
      type = "Fragment Shader";
      break;
    case GL_GEOMETRY_SHADER:
      type = "Geometry Shader";
      break;
    case GL_TESS_CONTROL_SHADER:
      type = "Tessellation Control Shader";
      break;
    case GL_TESS_EVALUATION_SHADER:
      type = "Tessellation Evaluation Shader";
      break;
    case GL_COMPUTE_SHADER:
      type = "Compute Shader";
      break;
    default:
      type = "Unknown Type Shader";
      break;
  }
  return type;
}

}  // namespace Aria