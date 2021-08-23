#include "Platform/Vulkan/Devices/LogicalDevice.hpp"

using namespace Worlds;

LogicalDevice::LogicalDevice(const Instance *instance, const Surface *surface,
                             const PhysicalDevice *physical_device)
    : m_instance(instance), m_surface(surface),
      m_physical_device(physical_device) {
    createLogicalDevice();
}

LogicalDevice::~LogicalDevice() { vkDestroyDevice(m_logical_device, nullptr); }

void LogicalDevice::createLogicalDevice() {
    findQueueFamilies(m_physical_device->getPhysicalDevice());

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {
        m_graphics_family, m_present_family, m_compute_family};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    m_enabled_features.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount =
        static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &m_enabled_features;

    createInfo.enabledExtensionCount =
        static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (enableValidationLayers) {
        createInfo.enabledLayerCount =
            static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(m_physical_device->getPhysicalDevice(), &createInfo,
                       nullptr, &m_logical_device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(m_logical_device, m_graphics_family, 0, &m_graphics_queue);
    vkGetDeviceQueue(m_logical_device, m_present_family, 0, &m_present_queue);
    vkGetDeviceQueue(m_logical_device, m_compute_family, 0, &m_compute_queue);
}

void LogicalDevice::findQueueFamilies(VkPhysicalDevice device) {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                             nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                             queueFamilies.data());

    std::optional<uint32_t> graphicsFamily, computeFamily, presentFamily;

    uint32_t foundFamily = 0;

    int i = 0;
    for (const auto &queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphicsFamily = i;
            m_graphics_family = i;
            supportedQueues |= VK_QUEUE_GRAPHICS_BIT;
        }

        if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
            computeFamily = i;
            m_compute_family = i;
            supportedQueues |= VK_QUEUE_COMPUTE_BIT;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface->getSurface(),
                                             &presentSupport);
        if (presentSupport) {
            presentFamily = i;
            m_present_family = i;
        }

        if (graphicsFamily && computeFamily && presentFamily) {
            break;
        }

        i++;
    }
}
