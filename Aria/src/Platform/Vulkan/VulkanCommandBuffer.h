#pragma once

#include <glm/glm.hpp>

namespace aria {
class VulkanCommandBuffer {
 public:
  VulkanCommandBuffer();

  // not doing anything in destructor - destroying command pool is sufficient
  // vk spec 1.3 pg. 201
  ~VulkanCommandBuffer() = default;

  bool StartRecording();
  void Draw(std::uint32_t vertex_count, std::uint32_t instance_count, std::uint32_t first_vertex,
            std::uint32_t first_instance);
  void StartRenderPass(glm::vec4 color = {0.0f, 0.0f, 0.0f, 1.0f});
  void EndRenderPass();
  bool Reset();
  bool Bind();
  bool EndRecording();

 private:
  //VkCommandBuffer command_buffer_ = nullptr;

  void SetViewport();
  void SetScissor();
};
}  // namespace ARIA