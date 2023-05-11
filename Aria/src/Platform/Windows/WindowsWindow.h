#pragma once

#include <stdint.h>
#include "Aria/Core/Window.h"
#include "Aria/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace ARIA {

class WindowsWindow : public Window {
 public:
  WindowsWindow(const WindowProps& props);
  virtual ~WindowsWindow();

  void on_update() override;

  inline unsigned int get_width() const override { return mData.mWidth; }
  inline unsigned int get_height() const override { return mData.mHeight; }

  // Window attributes
  inline void set_event_callback(const EventCallbackFn& callback) override { mData.mEventCallback = callback; }

  void set_vsync(bool enabled) override;
  inline bool is_vsync() const override { return mData.mVSync; }

  void* get_native_window() const override { return mWindow; }

 private:
  void init();
  void create_window();
  void shutdown();

 private:
  GLFWwindow* mWindow;
  GraphicsContext* mContext;

  struct WindowData {
    std::string mTitle;
    unsigned int mWidth, mHeight;
    bool mVSync;

    EventCallbackFn mEventCallback;
  };

  WindowData mData;
};
}  // namespace ARIA
