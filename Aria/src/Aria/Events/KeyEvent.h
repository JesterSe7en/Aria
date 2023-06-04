#pragma once

#include "Event.h"

namespace aria {

class ARIA_API KeyEvent : public Event {
 public:
  inline int GetKeyCode() const { return key_code_; }

  EVENT_CLASS_CATEGORY(EVENT_CATEGORY_KEYBOARD | EVENT_CATEGORY_INPUT)

 protected:
  KeyEvent(const int keycode) : key_code_(keycode) {}

  int key_code_;
};

class ARIA_API KeyPressedEvent : public KeyEvent {
 public:
  KeyPressedEvent(int keycode, int repeatCount) : KeyEvent(keycode), repeat_count_(repeatCount) {}

  inline int GetRepeatCount() { return repeat_count_; }

  inline std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyPressedEvent: " << key_code_ << " (" << repeat_count_ << " repeats)";
    return ss.str();
  }

  EVENT_CLASS_TYPE(KEY_PRESSED)

 private:
  unsigned int repeat_count_;
};

class ARIA_API KeyReleasedEvent : public KeyEvent {
 public:
  KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

  inline std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyReleasedEvent: " << key_code_;
    return ss.str();
  }

  EVENT_CLASS_TYPE(KEY_RELEASED)
};
}// namespace aria