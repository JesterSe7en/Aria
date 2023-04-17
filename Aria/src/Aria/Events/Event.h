#pragma once

#include "Aria/Core.h"
#include "ariapch.h"

namespace Aria {

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

#define EVENT_CLASS_TYPE(type)                                                \
  static EventType GetStaticType() { return EventType::##type; }              \
  virtual EventType GetEventType() const override { return GetStaticType(); } \
  virtual const char* GetName() const override { return #type; }
#define EVENT_CLASS_CATEGORY(category) \
  virtual int GetCategoryFlags() const override { return category; }

class ARIA_API Event {
 public:
  virtual EventType GetEventType() const = 0;
  virtual const char* GetName() const = 0;
  virtual int GetCategoryFlags() const = 0;
  virtual std::string ToString() const { return GetName(); }

  inline bool IsInCategory(EventCatagory category) {
    return GetCategoryFlags() & category;
  }
  bool Handled = false;
};

class EventDispatcher {
  template <typename T>
  using EventFn = std::is_function<bool(T&)>;

 public:
  EventDispatcher(Event& event) : m_Event(event) {}

  template <typename T, typename F>
  bool Dispatch(const F& func) {
    if (m_Event.GetEventType() == T::GetStaticType()) {
      m_Event.Handled |= func(static_cast<T&>(m_Event));
      return true;
    }
    return false;
  }

 private:
  Event& m_Event;
};

inline std::ostream& operator<<(std::ostream& os, const Event& e) {
  return os << e.ToString();
}
}  // namespace Aria