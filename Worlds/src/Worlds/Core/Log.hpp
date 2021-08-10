#pragma once

#include "Worlds/Core/Base.hpp"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>
#pragma warning(pop)

namespace Worlds {

class Log {
  public:
    static void init();

    static Ref<spdlog::logger> &GetCoreLogger() { return s_CoreLogger; }
    static Ref<spdlog::logger> &GetClientLogger() { return s_ClientLogger; }

  private:
    static Ref<spdlog::logger> s_CoreLogger;
    static Ref<spdlog::logger> s_ClientLogger;
};

} // namespace Worlds

// Core log macros
#define W_CORE_TRACE(...) ::Worlds::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define W_CORE_INFO(...) ::Worlds::Log::GetCoreLogger()->info(__VA_ARGS__)
#define W_CORE_WARN(...) ::Worlds::Log::GetCoreLogger()->warn(__VA_AGS__)
#define W_CORE_ERROR(...) ::Worlds::Log::GetCoreLogger()->error(__VA_ARGS__)
#define W_CORE_CRITICAL(...)                                                   \
    ::Worlds::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define W_TRACE(...) ::Worlds::Log::GetClientLogger()->trace(__VA_ARGS__)
#define W_INFO(...) ::Worlds::Log::GetClientLogger()->info(__VA_ARGS__)
#define W_WARN(...) ::Worlds::Log::GetClientLogger()->warn(__VA_ARGS__)
#define W_ERROR(...) ::Worlds::Log::GetClientLogger()->error(__VA_ARGS__)
#define W_CRITICAL(...) ::Worlds::Log::GetClientLogger()->critical(__VA_ARGS__)
