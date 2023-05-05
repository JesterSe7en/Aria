#include "ariapch.h"
#include "Texture.h"

#include <glad/gl.h>

namespace ARIA {
// OpenGL requires textures to be loaded from bottom left to top right

Texture::Texture() {}

Texture::~Texture() { glad_glDeleteTextures(1, &mRendererID); }

}  // namespace ARIA