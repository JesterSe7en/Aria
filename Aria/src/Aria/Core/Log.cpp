#include "ariapch.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace aria {
Ref<spdlog::logger> Log::core_logger_;
Ref<spdlog::logger> Log::client_logger_;

void Log::Init() {
  // see wiki https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
  spdlog::set_pattern("%^[%T] %n: [%l] - %v%$");

  core_logger_ = spdlog::stdout_color_mt("ARIA");
  core_logger_->set_level(spdlog::level::trace);

  client_logger_ = spdlog::stdout_color_mt("APP");
  client_logger_->set_level(spdlog::level::trace);
}
}  // namespace ARIA
