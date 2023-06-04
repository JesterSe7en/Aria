#pragma once

#include "Aria/Core/Base.h"
#include <string>
#include <unordered_map>

namespace aria {

enum class ShaderType {
  NONE,
  VERTEX,
  FRAGMENT,
  // add more later
};

class Shader {
 public:
  virtual ~Shader() = default;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual const std::string &GetName() const = 0;

  static Ref<Shader> Create(const std::string &name, const std::string &vertex_src, const std::string &fragment_src);
  static Ref<Shader> Create(const std::string &file_path);
  static Ref<Shader> Create(const std::string &file_path, ShaderType type);
};

class ShaderLibrary {
 public:
  void Add(const Ref<Shader> &shader);
  void Add(const std::string &name, const Ref<Shader> &shader);
  Ref<Shader> Load(const std::string &file_path);
  Ref<Shader> Load(const std::string &file_path, ShaderType type);
  Ref<Shader> Load(const std::string &name, const std::string &file_path);
  Ref<Shader> Get(const std::string &name);

  bool Exists(const std::string &name);

 private:
  std::unordered_map<std::string, Ref<Shader>> shaders_;
};
}// namespace aria
