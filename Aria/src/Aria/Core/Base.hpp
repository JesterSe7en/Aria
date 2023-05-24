#pragma once

#include <memory>

#ifdef ARIA_PLATFORM_WINDOWS
#if ARIA_DYNAMIC_LINK
#ifdef ARIA_BUILD_DLL
#define ARIA_API __declspec(dllexport)
#else
#define ARIA_API __declspec(dllimport)
#endif
#else
#define ARIA_API
#endif
#else
#error Aria only supports Windows!
#endif// ARIA_PLATFORM_WINDOWS

#ifdef ARIA_ENABLE_ASSERTS
#define ARIA_ASSERT(x, ...)                                                                                            \
  {                                                                                                                    \
    if (!(x)) {                                                                                                        \
      ARIA_ERROR("Assertion Failed: {0}", __VA_ARGS__);                                                                \
      __debugbreak();                                                                                                  \
    }                                                                                                                  \
  }
#define ARIA_CORE_ASSERT(x, ...)                                                                                       \
  {                                                                                                                    \
    if (!(x)) {                                                                                                        \
      ARIA_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__);                                                           \
      __debugbreak();                                                                                                  \
    }                                                                                                                  \
  }
#else
#define ARIA_ASSERT(x, ...)
#define ARIA_CORE_ASSERT(x, ...)
#endif// ARIA_ENABLE_ASSERTS

#define BIT(x) (1 << x)

#define ARIA_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace aria {
template<typename T>
using Scope = std::unique_ptr<T>;

// To extend it further
// template <typename T>
// class Scope {
// };

template<typename T>
using Ref = std::shared_ptr<T>;
}// namespace aria

#include "Aria/Core/Log.hpp"