#include "ariapch.h"
#include "Aria/Renderer/RendererApi.hpp"
#include "RendererApi.hpp"

namespace aria {
RendererApi::Api RendererApi::api_ = RendererApi::Api::NONE;

RendererApi::Api RendererApi::Create() { return Api(); }

void RendererApi::SetApi(Api api) { api_ = api; }
}// namespace aria