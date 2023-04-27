#include "ariapch.h"
#include "RendererAPI.h"

namespace ARIA {
RendererAPI::API RendererAPI::sAPI = RendererAPI::API::OpenGL;
RendererAPI::API RendererAPI::create() { return API(); }
}  // namespace ARIA