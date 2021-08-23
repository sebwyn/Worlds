#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Platform/Vulkan/Devices/LogicalDevice.hpp"
#include "Platform/Vulkan/Devices/PhysicalDevice.hpp"
#include "Platform/Vulkan/Devices/Surface.hpp"

namespace Worlds {

class Swapchain {
  public:
    Swapchain(GLFWwindow *window, const Surface *surface,
              const PhysicalDevice *physical_device,
              const LogicalDevice *logical_device);

    ~Swapchain();

    VkResult acquireNextImage(VkSemaphore presentComplete, VkFence flightFence);
    const VkSwapchainKHR &getSwapchain() { return m_swapchain; }

    uint32_t getCurrentImage() const { return m_current_image; }
    VkFormat getImageFormat() const { return m_image_format; }
    uint32_t getSize() const { return m_images.size(); }
    VkExtent2D getExtent() const { return m_extent; }
    const std::vector<VkImageView> &getImageViews() const { return m_image_views; }

  private:
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(
        const std::vector<VkPresentModeKHR> &availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
    void createImageViews();
    VkImageView createImageView(VkImage image, VkFormat format,
                                VkImageAspectFlags aspectFlags,
                                uint32_t mipLevels);

  private:
    GLFWwindow *m_window;
    const Surface *m_surface;
    const PhysicalDevice *m_physical_device;
    const LogicalDevice *m_logical_device;

    VkSwapchainKHR m_swapchain;

    std::vector<VkImage> m_images;
    VkFormat m_image_format;
    VkExtent2D m_extent;
    std::vector<VkImageView> m_image_views;

    uint32_t m_current_image = 0;
};

} // namespace Worlds
