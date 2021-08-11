#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Platform/Vulkan/Devices/Instance.hpp"

namespace Worlds {

class Surface {
  public:
    Surface(GLFWwindow* window, const Instance* instance);
    ~Surface();

    const VkSurfaceKHR &getSurface() const { return m_surface; };

  private:
    void createSurface();

  private:
    GLFWwindow* m_window;
    const Instance* m_instance;

    VkSurfaceKHR m_surface;
};

} // namespace Worlds
