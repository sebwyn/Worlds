#pragma once

#include "Worlds/Core/Base.hpp"
#include "Worlds/Core/Log.hpp"
#include <filesystem>

#ifdef W_ENABLE_ASSERTS

	// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
	// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
	#define W_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { HZ##type##ERROR(msg, __VA_ARGS__); W_DEBUGBREAK(); } }
	#define W_INTERNAL_ASSERT_WITH_MSG(type, check, ...) W_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define W_INTERNAL_ASSERT_NO_MSG(type, check) W_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", W_STRINGIFY_MACRO(check), std::filesystem:path(__FILE__).filename().string(), __LINE__)

	#define W_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define W_INTERNAL_ASSERT_GET_MACRO(...) W_EXPAND_MACRO( W_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, W_INTERNAL_ASSERT_WITH_MSG, W_INTERNAL_ASSERT_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define W_ASSERT(...) W_EXPAND_MACRO( W_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define W_CORE_ASSERT(...) W_EXPAND_MACRO( W_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define W_ASSERT(...)
	#define W_CORE_ASSERT(...)
#endif
