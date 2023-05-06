#include "ariapch.h"
#include "OpenGLTexture2D.h"

#include "stb_image.h"

#include <glad/gl.h>

namespace ARIA {

OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : mWidth(0), mHeight(0), mPath(path) {
  // use stb_img to load image
  // OpenGL expects to start at the bottom left to the top right of the image
  stbi_set_flip_vertically_on_load(1);

  // 4 channels = rgba
  int width, height, bpp;
  stbi_uc* data = stbi_load(path.c_str(), &width, &height, &bpp, 0);
  ARIA_CORE_ASSERT(data, "Unable to load texture from {0}", path);
  // ARIA_CORE_ASSERT(data == nullptr, "Unable to load texture from {0}", path);
  mWidth = width;
  mHeight = height;

  glad_glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
  glad_glTextureStorage2D(mRendererID, 1, GL_RGB8, mWidth, mHeight);

  // filtering should be exposed to the API
  glad_glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glad_glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // GL_RGB can be compared against the channels given from the stbi_load() func
  glad_glTextureSubImage2D(mRendererID, 0, 0, 0, mWidth, mHeight, bpp == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);

  if (data) {
    stbi_image_free(data);
  }

  // pulled form open gl proj
  // glad_glGenTextures(1, &mRendererID);
  // glad_glBindTexture(GL_TEXTURE_2D, mRendererID);

  // glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  // glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // // S and T is equivalent to X and Y in texture land
  // // Clamp = clip or truncate the texture if it doesn't fit
  // glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  // glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // glad_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  // glad_glBindTexture(GL_TEXTURE_2D, 0);

  // if (data) {
  //   stbi_image_free(data);
  // }
}

OpenGLTexture2D::~OpenGLTexture2D() { glad_glDeleteTextures(1, &mRendererID); }

void OpenGLTexture2D::bind(uint32_t slot) const {
  glad_glBindTextureUnit(slot, mRendererID);
  // from opengl project
  // glad_glActiveTexture(GL_TEXTURE0 + slot);
  // glad_glBindTexture(GL_TEXTURE_2D, mRendererID);
}

void OpenGLTexture2D::unbind() const { glad_glBindTexture(GL_TEXTURE_2D, 0); }

}  // namespace ARIA