#pragma once

#include "Event.h"

namespace Aria {

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
      : m_Width(width), m_Height(height) {}

  inline unsigned int GetWidth() const { return m_Width; }
  inline unsigned int GetHeight() const { return m_Height; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
    return ss.str();
  }

  EVENT_CLASS_TYPE(WindowResize)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)

 private:
  unsigned int m_Width;
  unsigned int m_Height;
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
}  // namespace Aria