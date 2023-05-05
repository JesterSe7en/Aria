#pragma once

namespace ARIA {
class Texture {
 public:
  Texture(const std::string path);
  ~Texture();

 private:
  uint32_t mRendererID;
};
}  // namespace ARIA