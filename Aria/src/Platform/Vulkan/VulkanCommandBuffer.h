#pragma once

#include "vulkan/vulkan_core.h"

#include <glm/glm.hpp>

namespace ARIA {
class VulkanCommandBuffer {
 public:
  VulkanCommandBuffer();

  // not doing anything in destructor - destroying command pool is sufficent
  // vk spec 1.3 pg. 201
  ~VulkanCommandBuffer() = default;

  bool start_recording();
  void draw(std::uint32_t vertexCount, std::uint32_t instanceCount, std::uint32_t firstVertex,
            std::uint32_t firstInstance);
  void start_render_pass(glm::vec4 color = {0.0f, 0.0f, 0.0f, 1.0f});
  void end_render_pass();
  bool reset();
  bool bind();
  bool end_recording();

 private:
  VkCommandBuffer mCommandBuffer;

  void set_viewport();
  void set_scissor();
};
}  // namespace ARIA