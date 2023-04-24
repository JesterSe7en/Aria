#pragma once

#include "Event.h"

namespace ARIA {

class ARIA_API MouseMovedEvent : public Event {
 public:
  MouseMovedEvent(float x, float y) : mMouseX(x), mMouseY(y) {}

  inline float get_x() const { return mMouseX; }
  inline float get_y() const { return mMouseY; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseMovedEvent: " << mMouseX << ", " << mMouseY;
    return ss.str();
  }

  EVENT_CLASS_TYPE(MouseMoved)
  EVENT_CLASS_CATEGORY(EventCatagoryMouse | EventCatagoryInput)

 private:
  float mMouseX, mMouseY;
};

class ARIA_API MouseScrolledEvent : public Event {
 public:
  MouseScrolledEvent(float xOffset, float yOffset)
      : mXOffset(xOffset), mYOffset(yOffset) {}

  inline float get_x_offset() const { return mXOffset; }
  inline float get_y_offset() const { return mYOffset; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseScrolledEvent: " << mXOffset << ", " << mYOffset;
    return ss.str();
  }

  EVENT_CLASS_TYPE(MouseScrolled)
  EVENT_CLASS_CATEGORY(EventCatagoryMouse | EventCatagoryInput)

 private:
  float mXOffset, mYOffset;
};

class ARIA_API MouseButtonEvent : public Event {
 public:
  inline int get_mouse_button() const { return mButton; }

  EVENT_CLASS_CATEGORY(EventCatagoryMouse | EventCatagoryInput)
 protected:
  MouseButtonEvent(int button) : mButton(button) {}

  int mButton;
};

class ARIA_API MouseButtonPressedEvent : public MouseButtonEvent {
 public:
  MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseButtonPressedEvent: " << mButton;
    return ss.str();
  }

  EVENT_CLASS_TYPE(MouseButtonPressed)
};

class ARIA_API MouseButtonReleasedEvent : public MouseButtonEvent {
 public:
  MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseButtonReleasedEvent: " << mButton;
    return ss.str();
  }

  EVENT_CLASS_TYPE(MouseButtonReleased)
};
}  // namespace ARIA