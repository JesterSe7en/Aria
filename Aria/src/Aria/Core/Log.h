#pragma once

#include "Base.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace ARIA {
class ARIA_API Log {
 public:
  static void init();

  inline static std::shared_ptr<spdlog::logger>& get_core_logger() {
    return sCoreLogger;
  }
  inline static std::shared_ptr<spdlog::logger>& get_client_logger() {
    return sClientLogger;
  }

 private:
  static std::shared_ptr<spdlog::logger> sCoreLogger;
  static std::shared_ptr<spdlog::logger> sClientLogger;
};
}  // namespace ARIA

// Core log macros
#define ARIA_CORE_CRITICAL(...) \
  ::ARIA::Log::get_core_logger()->critical(__VA_ARGS__)
#define ARIA_CORE_ERROR(...) ::ARIA::Log::get_core_logger()->error(__VA_ARGS__)
#define ARIA_CORE_WARN(...) ::ARIA::Log::get_core_logger()->warn(__VA_ARGS__)
#define ARIA_CORE_INFO(...) ::ARIA::Log::get_core_logger()->info(__VA_ARGS__)
#define ARIA_CORE_TRACE(...) ::ARIA::Log::get_core_logger()->trace(__VA_ARGS__)

// Client log macros
#define ARIA_CRITICAL(...) \
  ::ARIA::Log::get_client_logger()->critical(__VA_ARGS__)
#define ARIA_ERROR(...) ::ARIA::Log::get_client_logger()->error(__VA_ARGS__)
#define ARIA_WARN(...) ::ARIA::Log::get_client_logger()->warn(__VA_ARGS__)
#define ARIA_INFO(...) ::ARIA::Log::get_client_logger()->info(__VA_ARGS__)
#define ARIA_TRACE(...) ::ARIA::Log::get_client_logger()->trace(__VA_ARGS__)

// When putting this into a distribution build need to do something like this
// (disable the macros) #define ARIA_CORE_INFO i.e. define the macros with
// nothing