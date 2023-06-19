#include "ariapch.h"

#include "ImGuiLayer.h"
#include "ImGuiLayerOpenGL.h"
#include "ImGuiLayerVulkan.h"

#include "Aria/Core/Log.h"
#include "Aria/Renderer/RendererApi.h"

namespace aria {

Scope<ImGuiLayer> ImGuiLayer::Create() {
  auto current_api = RendererApi::GetApi();
  switch (current_api) {
    case RendererApi::Api::OPEN_GL:
      return std::make_unique<ImGuiLayerOpenGL>();
    case RendererApi::Api::DIRECT_X:
      ARIA_CORE_ASSERT(false, "Imgui: RendererAPI::DirectX is currently not supported!");
      return nullptr;
    case RendererApi::Api::VULKAN:
      ARIA_CORE_INFO("Imgui: RendererAPI::Vulkan is currently not supported!");
      return std::make_unique<ImGuiLayerVulkan>();
    case RendererApi::Api::NONE:
      ARIA_CORE_ASSERT(false, "Imgui: RendererAPI::None is currently not supported!");
      return nullptr;
    default:
      ARIA_CORE_ASSERT(false, "Imgui: Unknown RendererAPI!");
      return nullptr;
  }
}

ImGuiLayer::ImGuiLayer(const std::string &name) : Layer(name) {}

}// namespace aria