#include "ariapch.h"
#include "Shader.h"
#include "Aria/Log.h"

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

Shader::~Shader() { glDeleteProgram(renderer_id_); }

void Shader::Bind() const { glUseProgram(renderer_id_); }

void Shader::Unbind() const { glUseProgram(0); }

}  // namespace Aria