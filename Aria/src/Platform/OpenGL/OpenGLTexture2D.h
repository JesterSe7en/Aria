#pragma once

#include <stdint.h>
#include "Aria/Renderer/Texture.h"

namespace ARIA {
class OpenGLTexture2D : public Texture2D {
 public:
  explicit OpenGLTexture2D(const std::string& path);
  ~OpenGLTexture2D() override;

  uint32_t get_width() const override { return mWidth; }
  uint32_t get_height() const override { return mHeight; }

  void bind(uint32_t slot = 0) const override;
  void unbind() const override;

 private:
  uint32_t mRendererID;
  uint32_t mWidth = 0;
  uint32_t mHeight = 0;
  std::string mPath;
};
}  // namespace ARIA