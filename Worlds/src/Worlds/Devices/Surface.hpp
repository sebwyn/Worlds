#pragma once

#include <volk.h>

namespace Worlds {
class Instance;
class PhysicalDevice;

class Surface {
    friend class Graphics;

  public:
    Surface(const Instance *instance, const PhysicalDevice *physicalDevice);
    ~Surface();

    operator const VkSurfaceKHR &() const { return surface; }

    const VkSurfaceKHR &GetSurface() const { return surface; }
    const VkSurfaceCapabilitiesKHR &GetCapabilities() const {
        return capabilities;
    }
    const VkSurfaceFormatKHR &GetFormat() const { return format; }

  private:
    const Instance *instance;
    const PhysicalDevice *physicalDevice;

    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkSurfaceCapabilitiesKHR capabilities = {};
    VkSurfaceFormatKHR format = {};
};
} // namespace Worlds
