#include "Worlds/Renderer/RenderAPI.hpp"
#include "wpch.hpp"

#include "Platform/Vulkan/VKRenderAPI.hpp"

#define GLFW_INCLUDE_VULKAN 
#include <GLFW/glfw3.h>

namespace Worlds {

RenderAPI::API RenderAPI::s_API = RenderAPI::API::Vulkan;

Scope<RenderAPI> RenderAPI::create(void *window) {
    switch(s_API){
        case(API::None): W_CORE_ASSERT(false, "No Graphics API is not supported"); 
        case(API::Vulkan): return CreateScope<VKRenderAPI>(static_cast<GLFWwindow *>(window));
    }
}

} // namespace Worlds
