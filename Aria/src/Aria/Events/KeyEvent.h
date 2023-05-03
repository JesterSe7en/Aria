#pragma once

#include "Event.h"

namespace ARIA {

class ARIA_API KeyEvent : public Event {
 public:
  inline int get_key_code() const { return mKeyCode; }

  EVENT_CLASS_CATEGORY(EventCatagoryKeyboard | EventCatagoryInput)

 protected:
  KeyEvent(const int keycode) : mKeyCode(keycode) {}

  int mKeyCode;
};

class ARIA_API KeyPressedEvent : public KeyEvent {
 public:
  KeyPressedEvent(int keycode, int repeatCount) : KeyEvent(keycode), mRepeatCount(repeatCount) {}

  inline int get_repeat_count() { return mRepeatCount; }

  inline std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyPressedEvent: " << mKeyCode << " (" << mRepeatCount << " repeats)";
    return ss.str();
  }

  EVENT_CLASS_TYPE(KeyPressed)

 private:
  unsigned int mRepeatCount;
};

class ARIA_API KeyReleasedEvent : public KeyEvent {
 public:
  KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

  inline std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyReleasedEvent: " << mKeyCode;
    return ss.str();
  }

  EVENT_CLASS_TYPE(KeyReleased)
};
}  // namespace ARIA