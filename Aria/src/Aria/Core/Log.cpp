#include "ariapch.h"
#include "Log.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace aria {
Ref<spdlog::logger> Log::p_core_logger_;
Ref<spdlog::logger> Log::p_client_logger_;

void Log::Init() {
  // see wiki https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
  spdlog::set_pattern("%^[%T] %n: [%l] - %v%$");

  p_core_logger_ = spdlog::stdout_color_mt("ARIA");
  p_core_logger_->set_level(spdlog::level::trace);

  p_client_logger_ = spdlog::stdout_color_mt("APP");
  p_client_logger_->set_level(spdlog::level::trace);
}
}  // namespace ARIA
