#include "ariapch.h"
#include "Aria/Renderer/RendererApi.h"
#include "RendererApi.h"

namespace aria {
RendererApi::Api RendererApi::api_ = RendererApi::Api::NONE;

RendererApi::Api RendererApi::Create() { return Api(); }

void RendererApi::SetApi(Api api) { api_ = api; }
}// namespace aria