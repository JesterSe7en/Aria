#pragma once

#include "Event.h"

namespace Aria {
class ARIA_API KeyEvent : public Event {
 public:
  inline int GetKeyCode() const { return m_KeyCode; }

  EVENT_CLASS_CATEGORY(EventCatagoryKeyboard | EventCatagoryInput)

 protected:
  KeyEvent(int keycode) : m_KeyCode(keycode) {}

  int m_KeyCode;
};

class ARIA_API KeyPressedEvent : public KeyEvent {
 public:
  KeyPressedEvent(int keycode, int repeatCount)
      : KeyEvent(keycode), m_RepeatCount(repeatCount) {}

  inline int GetRepeatCount() { return m_RepeatCount; }

  inline std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount
       << " repeats)";
    return ss.str();
  }

  EVENT_CLASS_TYPE(KeyPressed)

 private:
  unsigned int m_RepeatCount;
};

class ARIA_API KeyReleasedEvent : public KeyEvent {
 public:
  KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

  inline std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyReleasedEvent: " << m_KeyCode;
    return ss.str();
  }

  EVENT_CLASS_TYPE(KeyReleased)
};
}  // namespace Aria