#pragma once

#include "Base.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace aria {
class ARIA_API Log {
 public:
  static void Init();

  inline static std::shared_ptr<spdlog::logger> &GetCoreLogger() { return p_core_logger_; }
  inline static std::shared_ptr<spdlog::logger> &GetClientLogger() { return p_client_logger_; }

 private:
  static std::shared_ptr<spdlog::logger> p_core_logger_;
  static std::shared_ptr<spdlog::logger> p_client_logger_;
};
}  // namespace ARIA

// Core log macros
#define ARIA_CORE_CRITICAL(...) ::aria::Log::GetCoreLogger()->critical(__VA_ARGS__);
#define ARIA_CORE_ERROR(...) ::aria::Log::GetCoreLogger()->error(__VA_ARGS__);
#define ARIA_CORE_WARN(...) ::aria::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define ARIA_CORE_INFO(...) ::aria::Log::GetCoreLogger()->info(__VA_ARGS__);
#define ARIA_CORE_TRACE(...) ::aria::Log::GetCoreLogger()->trace(__VA_ARGS__);

// Client log macros
#define ARIA_CRITICAL(...) ::aria::Log::GetClientLogger()->critical(__VA_ARGS__);
#define ARIA_ERROR(...) ::aria::Log::GetClientLogger()->error(__VA_ARGS__);
#define ARIA_WARN(...) ::aria::Log::GetClientLogger()->warn(__VA_ARGS__);
#define ARIA_INFO(...) ::aria::Log::GetClientLogger()->info(__VA_ARGS__);
#define ARIA_TRACE(...) ::aria::Log::GetClientLogger()->trace(__VA_ARGS__);

// When putting this into a distribution build need to do something like this
// (disable the macros) #define ARIA_CORE_INFO i.e. define the macros with
// nothing