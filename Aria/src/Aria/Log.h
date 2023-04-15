#pragma once

#include "Core.h"
#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Aria {
	class ARIA_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }


	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};
}

// Core log macros
#define ARIA_CORE_CRITICAL(...)	::Aria::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define ARIA_CORE_ERROR(...)		::Aria::Log::GetCoreLogger()->error(__VA_ARGS__)
#define ARIA_CORE_WARN(...)			::Aria::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ARIA_CORE_INFO(...)			::Aria::Log::GetCoreLogger()->info(__VA_ARGS__)
#define ARIA_CORE_TRACE(...)		::Aria::Log::GetCoreLogger()->trace(__VA_ARGS__)

// Client log macros
#define ARIA_CRITICAL(...)			::Aria::Log::GetClientLogger()->critical(__VA_ARGS__)
#define ARIA_ERROR(...)					::Aria::Log::GetClientLogger()->error(__VA_ARGS__)
#define ARIA_WARN(...)					::Aria::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ARIA_INFO(...)					::Aria::Log::GetClientLogger()->info(__VA_ARGS__)
#define ARIA_TRACE(...)					::Aria::Log::GetClientLogger()->trace(__VA_ARGS__)

// When putting this into a distribution build need to do something like this (disable the macros)
// #define ARIA_CORE_INFO
// i.e. define the macros with nothing