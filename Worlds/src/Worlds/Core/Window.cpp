#include "wpch.hpp"
#include "Worlds/Core/Window.hpp"

#ifdef W_PLATFORM_MACOS
    #include "Platform/Mac/MacWindow.hpp"
#elif W_PLATFORM_WINDOWS
    #include "Platform/Win/WinWindow.hpp"
#endif

using namespace Worlds;

Window* Window::instance;

Scope<Window> Window::create(const WindowProps& props){
    #ifdef W_PLATFORM_MACOS
        return CreateScope<MacWindow>(props);
    #elif W_PLATFORM_WINDOWS
        return CreateScope<WinWindow>(props);
    #else
        W_CORE_ASSERT(false, "Unknown Platform!");
        return nullptr;
    #endif
}

