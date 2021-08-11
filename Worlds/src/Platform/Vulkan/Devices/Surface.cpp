#include "Platform/Vulkan/Devices/Surface.hpp"

using namespace Worlds;

Surface::Surface(GLFWwindow *window, const Instance *instance)
    : m_window(window), m_instance(instance) {
    createSurface();
}

Surface::~Surface() {
    vkDestroySurfaceKHR(m_instance->getInstance(), m_surface, nullptr);
}

void Surface::createSurface() {
    if (glfwCreateWindowSurface(m_instance->getInstance(), m_window, nullptr,
                                &m_surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}
