#include "ariapch.h"
#include "OpenGLTexture2D.h"

#include "stb_image.h"

#include <glad/gl.h>
#include <filesystem>

namespace ARIA {

OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : mPath(path) {
  if (!std::filesystem::exists(path)) {
    ARIA_CORE_WARN("Cannot find texture file: {0}", path)
    return;
  }

  // use stb_img to load image
  // OpenGL expects to start at the bottom left to the top right of the image
  stbi_set_flip_vertically_on_load(1);

  // 4 channels = rgba
  int width;
  int height;
  int bpp;
  stbi_uc* data = stbi_load(path.c_str(), &width, &height, &bpp, 0);
  ARIA_CORE_ASSERT(data, "Unable to load texture from {0}", path)
  mWidth = width;
  mHeight = height;

  GLenum opengl_format = 0;
  GLenum data_format = 0;
  switch (bpp) {
    case 3:
      opengl_format = GL_RGB8;
      data_format = GL_RGB;
      break;
    case 4:
      opengl_format = GL_RGBA8;
      data_format = GL_RGBA;
      break;
    default:
      ARIA_CORE_ASSERT(false, "Unsupported texture format")
      break;
  }

  glad_glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
  // This is how opengl will store the color data - note the internalformat parameter
  glad_glTextureStorage2D(mRendererID, 1, opengl_format, mWidth, mHeight);

  // filtering should be exposed to the API
  glad_glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glad_glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // GL_RGB can be compared against the channels given from the stbi_load() func
  // GL_RGB/RGBA is the format of the actual data
  glad_glTextureSubImage2D(mRendererID, 0, 0, 0, mWidth, mHeight, data_format, GL_UNSIGNED_BYTE, data);

  if (data) {
    stbi_image_free(data);
  }
}

OpenGLTexture2D::~OpenGLTexture2D() { glad_glDeleteTextures(1, &mRendererID); }

void OpenGLTexture2D::bind(uint32_t slot) const { glad_glBindTextureUnit(slot, mRendererID); }

void OpenGLTexture2D::unbind() const { glad_glBindTexture(GL_TEXTURE_2D, 0); }

}  // namespace ARIA