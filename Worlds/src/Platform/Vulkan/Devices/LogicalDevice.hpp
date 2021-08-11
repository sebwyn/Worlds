#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Platform/Vulkan/Devices/Instance.hpp"
#include "Platform/Vulkan/Devices/Surface.hpp"
#include "Platform/Vulkan/Devices/PhysicalDevice.hpp"

#include "Platform/Vulkan/Devices/Base.hpp"

namespace Worlds {

class LogicalDevice {
  public:
    LogicalDevice(const Instance* instance, const Surface* surface, const PhysicalDevice* physical_device);
    ~LogicalDevice();

    void waitIdle() const { vkDeviceWaitIdle(m_logical_device); }

    const VkDevice& getLogicalDevice() const { return m_logical_device; }

    const uint32_t getGraphicsFamily() const { return m_graphics_family; }
    const uint32_t getPresentFamily() const { return m_present_family; }

    const VkQueue& getGraphicsQueue() const { return m_graphics_queue; }
    const VkQueue& getPresentQueue() const { return m_present_queue; }
  private:
    void createLogicalDevice();
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

  private:
    const Instance* m_instance;
    const Surface* m_surface;
    const PhysicalDevice* m_physical_device;

    VkDevice m_logical_device;

    uint32_t m_graphics_family;
    uint32_t m_present_family;

    VkQueue m_graphics_queue;
    VkQueue m_present_queue;
};

} // namespace Worlds
