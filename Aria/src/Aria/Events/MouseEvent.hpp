#pragma once

#include "Event.hpp"

namespace aria {

class ARIA_API MouseMovedEvent : public Event {
 public:
  MouseMovedEvent(float x, float y) : mouse_x_(x), mouse_y_(y) {}

  inline float GetX() const { return mouse_x_; }
  inline float GetY() const { return mouse_y_; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseMovedEvent: " << mouse_x_ << ", " << mouse_y_;
    return ss.str();
  }

  EVENT_CLASS_TYPE(MouseMoved)
  EVENT_CLASS_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT)

 private:
  float mouse_x_, mouse_y_;
};

class ARIA_API MouseScrolledEvent : public Event {
 public:
  MouseScrolledEvent(float x_offset, float y_offset)
      : x_offset_(x_offset), y_offset_(y_offset) {}

  inline float GetXOffset() const { return x_offset_; }
  inline float GetYOffset() const { return y_offset_; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseScrolledEvent: " << x_offset_ << ", " << y_offset_;
    return ss.str();
  }

  EVENT_CLASS_TYPE(MouseScrolled)
  EVENT_CLASS_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT)

 private:
  float x_offset_, y_offset_;
};

class ARIA_API MouseButtonEvent : public Event {
 public:
  inline int GetMouseButton() const { return button_; }

  EVENT_CLASS_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT)
 protected:
  MouseButtonEvent(int button) : button_(button) {}

  int button_;
};

class ARIA_API MouseButtonPressedEvent : public MouseButtonEvent {
 public:
  MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseButtonPressedEvent: " << button_;
    return ss.str();
  }

  EVENT_CLASS_TYPE(MOUSE_BUTTON_PRESSED)
};

class ARIA_API MouseButtonReleasedEvent : public MouseButtonEvent {
 public:
  MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseButtonReleasedEvent: " << button_;
    return ss.str();
  }

  EVENT_CLASS_TYPE(MOUSE_BUTTON_RELEASED)
};
}  // namespace ARIA