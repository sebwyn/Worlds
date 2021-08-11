#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Platform/Vulkan/Devices/Instance.hpp"
#include "Platform/Vulkan/Devices/Surface.hpp"

#include "Platform/Vulkan/Devices/Base.hpp"

namespace Worlds {

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class PhysicalDevice {
  public:
    PhysicalDevice(const Instance* instance, const Surface* surface);
    ~PhysicalDevice();

    const VkPhysicalDevice& getPhysicalDevice() const { return m_physical_device; }
    const VkSampleCountFlagBits& getSampleCount() const { return m_msaaSamples; }
    SwapChainSupportDetails querySwapChainSupport();
  private:
    void pickPhysicalDevice();
    bool isDeviceSuitable(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    VkSampleCountFlagBits getMaxUsableSampleCount();
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
  private:
    const Instance* m_instance;
    const Surface* m_surface;

    VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
    VkSampleCountFlagBits m_msaaSamples = VK_SAMPLE_COUNT_1_BIT;
};

} // namespace Worlds
