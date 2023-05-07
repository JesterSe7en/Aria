#pragma once

#include <string>
#include <unordered_map>
#include "Aria/Core/Base.h"

namespace ARIA {

class Shader {
 public:
  virtual ~Shader() = default;

  virtual void bind() const = 0;
  virtual void unbind() const = 0;

  virtual const std::string get_name() const = 0;

  static Ref<Shader> Create(const std::string& name, const std::string& vertex_src, const std::string& fragment_src);
  static Ref<Shader> Create(const std::string& file_path);
};

class ShaderLibrary {
 public:
  void add(const Ref<Shader>& shader);
  void add(const std::string& name, const Ref<Shader>& shader);
  Ref<Shader> load(const std::string& file_path);
  Ref<Shader> load(const std::string& name, const std::string& file_path);
  Ref<Shader> get(const std::string& name);

  bool exists(const std::string& name);

 private:
  std::unordered_map<std::string, Ref<Shader>> mShaders;
};
}  // namespace ARIA
