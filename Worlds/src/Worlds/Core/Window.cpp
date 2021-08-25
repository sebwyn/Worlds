#include "Worlds/Core/Window.hpp"
#include "wpch.hpp"

#ifdef W_PLATFORM_MACOS
#include "Platform/Mac/MacWindow.hpp"
//#elif W_PLATFORM_WINDOWS
//#include "Platform/Win/WinWindow.hpp"
#endif

using namespace Worlds;

WindowAPI::WindowAPI() {
    WindowProps props;
    // TODO: set props based on a call to App::GetWindowProps or something

#ifdef W_PLATFORM_MACOS
    window = CreateScope<MacWindow>(props);
//#elif W_PLATFORM_WINDOWS
//    window = CreateScope<WinWindow>(props);
#else
    W_CORE_ASSERT(false, "Platform not supported!");
    return nullptr;
#endif
}
