#pragma once

#include <glm/glm.hpp>

#include "Aria/Renderer/RendererAPI.h"
#include "vulkan/vulkan_core.h"

namespace ARIA {
class VulkanRendererAPI : public RendererAPI {
 public:
  void init() override;
  void clear() override;
  void set_clear_color(const glm::vec4 color) override;
  void draw_indexed(const Ref<VertexArray>& vertex_array) override;

 private:
  void create_instance();
  bool has_validation_support() const;

  const std::vector<const char*> mValidationLayers = {"VK_LAYER_KHRONOS_validation"};
  VkInstance mInstance;
};
}  // namespace ARIA