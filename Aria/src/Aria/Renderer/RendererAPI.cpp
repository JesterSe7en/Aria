#include "Aria/Renderer/RendererAPI.h"
#include "ariapch.h"
#include "RendererAPI.h"

namespace ARIA {
RendererAPI::API RendererAPI::sAPI = RendererAPI::API::None;
RendererAPI::API RendererAPI::create() { return API(); }

void RendererAPI::set_api(API api) { sAPI = api; }
}  // namespace ARIA