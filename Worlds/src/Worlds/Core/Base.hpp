#pragma once

#include <memory>

#include "Worlds/Core/PlatformDetection.hpp"

namespace Worlds {

template <typename T> using Scope = std::unique_ptr<T>;
template <typename T, typename... Args>
constexpr Scope<T> CreateScope(Args... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T> using Ref = std::shared_ptr<T>;
template <typename T, typename... Args>
constexpr Ref<T> CreateRef(Args... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

} // namespace Worlds

#define BIT(x) (1 << x)

#define W_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#include "Worlds/Core/Asserts.hpp"
#include "Worlds/Core/Log.hpp"
