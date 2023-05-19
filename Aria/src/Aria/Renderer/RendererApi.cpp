#include "ariapch.h"
#include "RendererApi.h"

namespace aria {
RendererApi::Api RendererApi::api_ = RendererApi::Api::OPEN_GL;
RendererApi::Api RendererApi::Create() { return Api(); }
}  // namespace ARIA