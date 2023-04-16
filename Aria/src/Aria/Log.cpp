#include "ariapch.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"


namespace Aria {
  std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
  std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

  void Log::Init() {
    // see wiki https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
    spdlog::set_pattern("%^[%T] %n: [%l] - %v%$");

    s_CoreLogger = spdlog::stdout_color_mt("ARIA");
    s_CoreLogger->set_level(spdlog::level::trace);

    s_ClientLogger = spdlog::stdout_color_mt("APP");
    s_ClientLogger->set_level(spdlog::level::trace);
  }
}
