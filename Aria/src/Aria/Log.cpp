#include "ariapch.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace ARIA {
Ref<spdlog::logger> Log::sCoreLogger;
Ref<spdlog::logger> Log::sClientLogger;

void Log::init() {
  // see wiki https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
  spdlog::set_pattern("%^[%T] %n: [%l] - %v%$");

  sCoreLogger = spdlog::stdout_color_mt("ARIA");
  sCoreLogger->set_level(spdlog::level::trace);

  sClientLogger = spdlog::stdout_color_mt("APP");
  sClientLogger->set_level(spdlog::level::trace);
}
}  // namespace ARIA
