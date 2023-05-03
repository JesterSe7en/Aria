#include "ariapch.h"
#include "Shader.h"
#include "Aria/Log.h"

#include <filesystem>
#include <glad/gl.h>

namespace ARIA {

Shader::Shader(const std::string &vertex_src, const std::string &fragment_src)
    : mRendererID(0) {
  // Pulled from Khrono's documentation
  // --------------- Vertex Shader ---------------

  // create an empty vertex shader handle
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
    std::vector<char> info_log(max_length);
    glGetShaderInfoLog(vertex_shader, max_length, &max_length, &info_log[0]);

    // We don't need the shader anymore.
    glDeleteShader(vertex_shader);

    // Use the info_log as you see fit.
    ARIA_CORE_ERROR("Vertex shader compile error: {0} - Source: {1}",
                    info_log.data(), vertex_src);
    ARIA_CORE_ASSERT(false);
    return;
  }

  // --------------- Fragment Shader ---------------

  // create an empty fragment shader handle
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
    std::vector<char> info_log(max_length);
    glGetShaderInfoLog(fragment_shader, max_length, &max_length, &info_log[0]);

    // We don't need the shader anymore.
    glDeleteShader(fragment_shader);
    // Either of them. Don't leak shaders.
    glDeleteShader(vertex_shader);

    // Use the info_log as you see fit.
    ARIA_CORE_ERROR("Fragment shader compile error: {0} - Source: {1}",
                    info_log.data(), fragment_src);
    ARIA_CORE_ASSERT(false);
    return;
  }

  // --------------- Creating Shader Program ---------------

  // Vertex and fragment shaders are successfully compiled.
  // Now time to link them together into a program.
  // get a program object.
  mRendererID = glCreateProgram();

  // Attach our shaders to our program
  glAttachShader(mRendererID, vertex_shader);
  glAttachShader(mRendererID, fragment_shader);

  // Link our program
  glLinkProgram(mRendererID);

  // Note the different functions here: glGetProgram* instead of glGetShader*.
  int is_linked = 0;
  glGetProgramiv(mRendererID, GL_LINK_STATUS, (int *)&is_linked);
  if (is_linked == GL_FALSE) {
    int max_length = 0;
    glGetProgramiv(mRendererID, GL_INFO_LOG_LENGTH, &max_length);

    // The max_length includes the NULL character
    std::vector<char> infoLog(max_length);
    glGetProgramInfoLog(mRendererID, max_length, &max_length, &infoLog[0]);

    // We don't need the program anymore.
    glDeleteProgram(mRendererID);
    // Don't leak shaders either.
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // Use the info_log as you see fit.
    ARIA_CORE_ERROR("Shader linking error: {0}", infoLog.data());
    ARIA_CORE_ASSERT(false);
    // In this simple program, we'll just leave
    return;
  }

  // Always detach shaders after a successful link.
  glDetachShader(mRendererID, vertex_shader);
  glDetachShader(mRendererID, fragment_shader);
}

Shader::Shader(const std::string &file_path) : mRendererID(0) {
  if (!std::filesystem::exists(file_path)) {
    ARIA_CORE_WARN("Cannot find shader file: {0}", file_path);
  };

  ShaderProgramSrc src = parse_shader(file_path);
  mRendererID = create_shader(src.vertex_source, src.fragment_source);
}

Shader::~Shader() { glDeleteProgram(mRendererID); }

void Shader::bind() const { glUseProgram(mRendererID); }

void Shader::unbind() const { glUseProgram(0); }

void Shader::set_uniform_1i(const std::string &name, int value) {
  glad_glUniform1i(get_uniform_location(name), value);
}

void Shader::set_uniform_4f(const std::string &name, float v0, float v1,
                            float v2, float v3) {
  glad_glUniform4f(get_uniform_location(name), v0, v1, v2, v3);
}

void Shader::set_uniform_mat4f(const std::string &name,
                               const glm::mat4 &matrix) {
  glad_glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE,
                          &matrix[0][0]);
}

// TODO: only supports vertex and frag shaders
ShaderProgramSrc Shader::parse_shader(const std::string &file_path) {
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

uint32_t Shader::compile_shader(unsigned int type, const std::string &source) {
  // create an empty vertex shader handle
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

    // Use the info_log as you see fit.
    ARIA_CORE_ERROR("Shader compile error: {0} - Type: {1} - Source: {2}",
                    infoLog.data(), type, src);
    ARIA_CORE_ASSERT(false);
    return 0;
  }
  return id;
}

// TODO: accept other kinds of shaders, prob pass a vector
uint32_t Shader::create_shader(const std::string &vertex_shader,
                               const std::string &fragment_shader) {
  uint32_t id = glCreateProgram();
  uint32_t vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
  uint32_t fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

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

    // Use the info_log as you see fit.
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

const char *Shader::get_shader_type(const int shader_type) const {
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

int Shader::get_uniform_location(const std::string &name) {
  if (mUniformLocationCache.find(name) != mUniformLocationCache.end())
    return mUniformLocationCache[name];

  int location = glad_glGetUniformLocation(mRendererID, name.c_str());
  if (location == -1)
    ARIA_CORE_WARN("Warning: Uniform '{0}' does not exist", name);

  mUniformLocationCache[name] = location;
  return location;
}

} // namespace ARIA
