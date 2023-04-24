#pragma once

#include "Event.h"

namespace ARIA {

class ARIA_API WindowCloseEvent : public Event {
 public:
  WindowCloseEvent() {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "WindowCloseEvent Triggered";
    return ss.str();
  }

  EVENT_CLASS_TYPE(WindowClosed)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class ARIA_API WindowResizeEvent : public Event {
 public:
  WindowResizeEvent(unsigned int width, unsigned int height)
      : mWidth(width), mHeight(height) {}

  inline unsigned int get_width() const { return mWidth; }
  inline unsigned int get_height() const { return mHeight; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "WindowResizeEvent: " << mWidth << ", " << mHeight;
    return ss.str();
  }

  EVENT_CLASS_TYPE(WindowResize)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)

 private:
  unsigned int mWidth;
  unsigned int mHeight;
};

class ARIA_API WindowFocusEvent : public Event {
 public:
  WindowFocusEvent() {}

  EVENT_CLASS_TYPE(WindowFocus)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class ARIA_API WindowLostFocusEvent : public Event {
 public:
  WindowLostFocusEvent() {}

  EVENT_CLASS_TYPE(WindowLostFocus)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class ARIA_API WindowMovedEvent : public Event {
 public:
  WindowMovedEvent() {}

  EVENT_CLASS_TYPE(WindowMoved)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class ARIA_API AppTickEvent : public Event {
 public:
  AppTickEvent() {}

  EVENT_CLASS_TYPE(AppTick)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class ARIA_API AppUpdateEvent : public Event {
 public:
  AppUpdateEvent() {}

  EVENT_CLASS_TYPE(AppUpdate)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class ARIA_API AppRenderEvent : public Event {
 public:
  AppRenderEvent() {}

  EVENT_CLASS_TYPE(AppRender)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};
}  // namespace ARIA