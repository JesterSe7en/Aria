#include "Aria/Renderer/RenderCommand.h"
#include "Aria/Core/Base.h"
#include "Aria/Renderer/RendererAPI.h"
#include "Platform/Vulkan/VulkanRendererAPI.h"
#include "ariapch.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Platform/Vulkan/VulkanRendererAPI.h"

#include "RenderCommand.h"

namespace ARIA {

RendererAPI* RenderCommand::sRendererAPI = nullptr;

void RenderCommand::init(RendererAPI::API api) {
  switch (api) {
    case RendererAPI::API::OpenGL:
      sRendererAPI = new OpenGLRendererAPI;
      break;
    case RendererAPI::API::Vulkan:
      sRendererAPI = new VulkanRendererAPI;
      // set to vulkan API;
      break;
    default:
      ARIA_CORE_ASSERT(false, "Attempted to initialized unknown API")
      return;
  }

  sRendererAPI->init();
}
}  // namespace ARIA