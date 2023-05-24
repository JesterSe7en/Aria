#pragma once

#include "Aria/Renderer/Texture.hpp"
#include <cstdint>

namespace aria {
class OpenGlTexture2D : public Texture2D {
 public:
  explicit OpenGlTexture2D(const std::string &path);
  ~OpenGlTexture2D() override;

  std::uint32_t GetWidth() const override { return width_; }
  std::uint32_t GetHeight() const override { return height_; }

  void Bind(uint32_t slot = 0) const override;
  void Unbind() const override;

 private:
  uint32_t renderer_id_;
  uint32_t width_ = 0;
  uint32_t height_ = 0;
  std::string path_;
};
}// namespace aria