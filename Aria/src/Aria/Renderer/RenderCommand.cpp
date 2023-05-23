#include "ariapch.h"

#include "Aria/Renderer/RenderCommand.hpp"

#include "Aria/Core/Base.hpp"
#include "Aria/Renderer/RendererApi.hpp"
#include "Platform/OpenGL/OpenGlRendererApi.hpp"
#include "Platform/Vulkan/VulkanRendererApi.hpp"

#include <memory>

namespace aria {

std::unique_ptr<RendererApi> RenderCommand::p_renderer_api_ = nullptr;

void RenderCommand::Init(RendererApi::Api api) {
  switch (api) {
    case RendererApi::Api::OPEN_GL:p_renderer_api_ = std::make_unique<OpenGlRendererApi>();
      break;
    case RendererApi::Api::VULKAN:p_renderer_api_ = std::make_unique<VulkanRendererApi>();
      break;
    default: ARIA_CORE_ASSERT(false, "Attempted to initialized unknown API")
      return;
  }

  p_renderer_api_->Init();
}

void RenderCommand::CreatePipeline() { p_renderer_api_->CreatePipeline(); }
}  // namespace ARIA
