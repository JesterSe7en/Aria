#pragma once

#include "Aria/Renderer/VertexArray.h"
#include <glm/glm.hpp>

namespace aria {
class RendererApi {
 public:
  enum class Api {
    // DirectX and Vulkan would be added later
    NONE = 0,
    OPEN_GL = 1,
    DIRECT_X = 2,
    VULKAN = 3,
  };

 public:
  virtual void Init() = 0;
  virtual void Clear() = 0;
  virtual void SetClearColor(const glm::vec4 color) = 0;
  virtual void DrawIndexed(const Ref<VertexArray> &array) = 0;

  static Api GetApi() { return api_; }
  static void SetApi(Api api);

  static Api Create();

  // vulkan commands
  virtual void BeginRecording(){};
  virtual void EndRecording(){};
  virtual void CmdBeginRenderPass(){};
  virtual void CmdEndRenderPass(){};
  virtual void CmdBindToGraphicsPipeline(){};
  virtual void CmdSetViewport(){};
  virtual void CmdSetScissor(){};
  virtual void CmdDraw(){};
  virtual void DrawFrame(){};
  virtual void CreateCommandModule(){};

 private:
  static Api api_;
};
}// namespace aria
