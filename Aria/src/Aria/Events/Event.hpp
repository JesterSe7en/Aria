#pragma once

#include "ariapch.h"

#include "Aria/Core/Base.hpp"

namespace aria {

// Events are blocking

enum class EventType {
  NONE = 0,
  WINDOW_CLOSED,
  WINDOW_RESIZE,
  WINDOW_FOCUS,
  WINDOW_LOST_FOCUS,
  WINDOW_MOVED,

  APP_TICK,
  APP_UPDATE,
  APP_RENDER,

  KEY_PRESSED,
  KEY_RELEASED,
  MOUSE_BUTTON_PRESSED,
  MOUSE_BUTTON_RELEASED,
  MouseMoved,
  MouseScrolled,
};

enum EventCategory {
  NONE = 0,
  EVENT_CATEGORY_APPLICATION = BIT(1),
  EVENT_CATEGORY_INPUT = BIT(2),
  EVENT_CATEGORY_KEYBOARD = BIT(3),
  EVENT_CATEGORY_MOUSE = BIT(4)
};

#define EVENT_CLASS_TYPE(type)                                     \
  static EventType GetStaticType() { return EventType::type; } \
  virtual EventType GetEventType() const override {              \
    return GetStaticType();                                      \
  }                                                                \
  virtual const char* GetName() const override { return #type; }
#define EVENT_CLASS_CATEGORY(category) \
  virtual int GetCategoryFlags() const override { return category; }

class ARIA_API Event {
 public:
  virtual EventType GetEventType() const = 0;
  virtual const char *GetName() const = 0;
  virtual int GetCategoryFlags() const = 0;
  virtual std::string ToString() const { return GetName(); }

  inline bool IsInCategory(EventCategory category) {
    return GetCategoryFlags() & category;
  }
  bool handled_ = false;
};

class EventDispatcher {
  template<typename T>
  using EventFn = std::is_function<bool(T &)>;

 public:
  EventDispatcher(Event &event) : event_(event) {}

  template<typename T, typename F>
  bool dispatch(const F &func) {
    if (event_.GetEventType() == T::GetStaticType()) {
      event_.handled_ |= func(static_cast<T &>(event_));
      return true;
    }
    return false;
  }

 private:
  Event &event_;
};

inline std::ostream &operator<<(std::ostream &os, const Event &e) {
  return os << e.ToString();
}
}  // namespace ARIA
