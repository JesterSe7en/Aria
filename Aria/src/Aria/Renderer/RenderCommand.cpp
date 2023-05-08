#include "Aria/Renderer/RenderCommand.h"
#include <memory>
#include "Aria/Core/Base.h"
#include "Aria/Renderer/RendererAPI.h"
#include "Platform/Vulkan/VulkanRendererAPI.h"
#include "ariapch.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Platform/Vulkan/VulkanRendererAPI.h"

#include "RenderCommand.h"

namespace ARIA {

std::unique_ptr<RendererAPI> RenderCommand::sRendererAPI = nullptr;

void RenderCommand::init(RendererAPI::API api) {
  switch (api) {
    case RendererAPI::API::OpenGL:
      sRendererAPI = std::make_unique<OpenGLRendererAPI>();
      break;
    case RendererAPI::API::Vulkan:
      sRendererAPI = std::make_unique<VulkanRendererAPI>();
      break;
    default:
      ARIA_CORE_ASSERT(false, "Attempted to initialized unknown API")
      return;
  }

  sRendererAPI->init();
}
}  // namespace ARIA