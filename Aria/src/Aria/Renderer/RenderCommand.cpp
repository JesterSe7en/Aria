#include "ariapch.h"
#include "Aria/Renderer/RenderCommand.h"
#include "Aria/Core/Base.h"
#include "Aria/Renderer/RendererApi.h"
#include "Platform/OpenGL/OpenGlRendererApi.h"
#include "Platform/Vulkan/VulkanRendererApi.h"

#include <memory>

namespace aria {

Scope<RendererApi> RenderCommand::p_renderer_api_ = nullptr;

Scope<RendererApi> RenderCommand::Init(RendererApi::Api api) {
  switch (api) {
    case RendererApi::Api::OPEN_GL:
      p_renderer_api_ = std::make_unique<OpenGlRendererApi>();
      break;
    case RendererApi::Api::VULKAN:
      p_renderer_api_ = std::make_unique<VulkanRendererApi>();
      break;
    default:
      ARIA_CORE_ASSERT(false, "Attempted to initialized unsupported API")
      return nullptr;
  }

  p_renderer_api_->Init();
  return std::move(p_renderer_api_);
}

}// namespace aria
