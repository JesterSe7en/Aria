#pragma once

#include <string>

namespace ARIA {

class Shader {
 public:
  virtual ~Shader() = default;

  virtual void bind() const = 0;
  virtual void unbind() const = 0;

  static Shader *Create(const std::string &vertex_src, const std::string &fragment_src);
  static Shader *Create(const std::string &file_path);
};
}  // namespace ARIA
