#pragma once

#include "Aria/Core.h"
#include "ariapch.h"

namespace ARIA {

// Events are blocking

enum class EventType {
  None = 0,
  WindowClosed,
  WindowResize,
  WindowFocus,
  WindowLostFocus,
  WindowMoved,

  AppTick,
  AppUpdate,
  AppRender,

  KeyPressed,
  KeyReleased,
  MouseButtonPressed,
  MouseButtonReleased,
  MouseMoved,
  MouseScrolled,
};

enum EventCatagory {
  None = 0,
  EventCategoryApplication = BIT(1),
  EventCatagoryInput = BIT(2),
  EventCatagoryKeyboard = BIT(3),
  EventCatagoryMouse = BIT(4)
};

#define EVENT_CLASS_TYPE(type)                                     \
  static EventType get_static_type() { return EventType::##type; } \
  virtual EventType get_event_type() const override {              \
    return get_static_type();                                      \
  }                                                                \
  virtual const char* get_name() const override { return #type; }
#define EVENT_CLASS_CATEGORY(category) \
  virtual int get_category_flags() const override { return category; }

class ARIA_API Event {
 public:
  virtual EventType get_event_type() const = 0;
  virtual const char* get_name() const = 0;
  virtual int get_category_flags() const = 0;
  virtual std::string ToString() const { return get_name(); }

  inline bool is_in_category(EventCatagory category) {
    return get_category_flags() & category;
  }
  bool Handled = false;
};

class EventDispatcher {
  template <typename T>
  using EventFn = std::is_function<bool(T&)>;

 public:
  EventDispatcher(Event& event) : mEvent(event) {}

  template <typename T, typename F>
  bool dispatch(const F& func) {
    if (mEvent.get_event_type() == T::get_static_type()) {
      mEvent.Handled |= func(static_cast<T&>(mEvent));
      return true;
    }
    return false;
  }

 private:
  Event& mEvent;
};

inline std::ostream& operator<<(std::ostream& os, const Event& e) {
  return os << e.ToString();
}
}  // namespace ARIA