#pragma once

#include "Event.hpp"

namespace aria {

class ARIA_API WindowCloseEvent : public Event {
 public:
  WindowCloseEvent() = default;

  std::string ToString() const override {
    std::stringstream ss;
    ss << "WindowCloseEvent Triggered";
    return ss.str();
  }

  EVENT_CLASS_TYPE(WINDOW_CLOSED)
  EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
};

class ARIA_API WindowResizeEvent : public Event {
 public:
  WindowResizeEvent(unsigned int width, unsigned int height)
      : width_(width), height_(height) {}

  inline unsigned int GetWidth() const { return width_; }
  inline unsigned int GetHeight() const { return height_; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "WindowResizeEvent: " << width_ << ", " << height_;
    return ss.str();
  }

  EVENT_CLASS_TYPE(WINDOW_RESIZE)
  EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)

 private:
  unsigned int width_;
  unsigned int height_;
};

class ARIA_API WindowFocusEvent : public Event {
 public:
  WindowFocusEvent() = default;

  EVENT_CLASS_TYPE(WINDOW_FOCUS)
  EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
};

class ARIA_API WindowLostFocusEvent : public Event {
 public:
  WindowLostFocusEvent() = default;

  EVENT_CLASS_TYPE(WINDOW_LOST_FOCUS)
  EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
};

class ARIA_API WindowMovedEvent : public Event {
 public:
  WindowMovedEvent() = default;

  EVENT_CLASS_TYPE(WINDOW_MOVED)
  EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
};

class ARIA_API AppTickEvent : public Event {
 public:
  AppTickEvent() = default;

  EVENT_CLASS_TYPE(APP_TICK)
  EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
};

class ARIA_API AppUpdateEvent : public Event {
 public:
  AppUpdateEvent() = default;

  EVENT_CLASS_TYPE(APP_UPDATE)
  EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
};

class ARIA_API AppRenderEvent : public Event {
 public:
  AppRenderEvent() = default;

  EVENT_CLASS_TYPE(APP_RENDER)
  EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
};
}  // namespace ARIA
