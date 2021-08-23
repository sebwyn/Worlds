#include "Platform/Vulkan/Renderpass/Swapchain.hpp"

#include "Platform/Vulkan/VKRenderAPI.hpp"

using namespace Worlds;

Swapchain::Swapchain(GLFWwindow *window, const Surface *surface,
                     const PhysicalDevice *physical_device,
                     const LogicalDevice *logical_device)
    : m_window(window), m_surface(surface), m_physical_device(physical_device),
      m_logical_device(logical_device) {

    SwapChainSupportDetails swapChainSupport =
        m_physical_device->querySwapchainSupport();

    VkSurfaceFormatKHR surfaceFormat =
        chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode =
        chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 &&
        imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = m_surface->getSurface();

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t queueFamilyIndices[] = {m_logical_device->getGraphicsFamily(),
                                     m_logical_device->getPresentFamily()};

    if (m_logical_device->getGraphicsFamily() !=
        m_logical_device->getPresentFamily()) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    if (vkCreateSwapchainKHR(m_logical_device->getLogicalDevice(), &createInfo,
                             nullptr, &m_swapchain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(m_logical_device->getLogicalDevice(), m_swapchain,
                            &imageCount, nullptr);
    m_images.resize(imageCount);
    vkGetSwapchainImagesKHR(m_logical_device->getLogicalDevice(), m_swapchain,
                            &imageCount, m_images.data());

    m_image_format = surfaceFormat.format;
    m_extent = extent;

    createImageViews();
}

VkSurfaceFormatKHR Swapchain::chooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR> &availableFormats) {
    for (const auto &availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkResult Swapchain::acquireNextImage(VkSemaphore presentComplete,
                                     VkFence flightFence) {
    vkWaitForFences(m_logical_device->getLogicalDevice(), 1, &flightFence,
                    VK_TRUE, UINT64_MAX);

    VkResult result = vkAcquireNextImageKHR(
        m_logical_device->getLogicalDevice(), m_swapchain, UINT64_MAX,
        presentComplete, VK_NULL_HANDLE, &m_current_image);

    return result;
}

VkPresentModeKHR Swapchain::chooseSwapPresentMode(
    const std::vector<VkPresentModeKHR> &availablePresentModes) {
    for (const auto &availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

Swapchain::~Swapchain() {
    vkDestroySwapchainKHR(m_logical_device->getLogicalDevice(), m_swapchain,
                          nullptr);

    for (const auto &image_view : m_image_views) {
        vkDestroyImageView(m_logical_device->getLogicalDevice(), image_view,
                           nullptr);
    }
}

VkExtent2D
Swapchain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) {
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);

        VkExtent2D actualExtent = {static_cast<uint32_t>(width),
                                   static_cast<uint32_t>(height)};

        actualExtent.width =
            std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                       capabilities.maxImageExtent.width);
        actualExtent.height =
            std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                       capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

void Swapchain::createImageViews() {
    m_image_views.resize(m_images.size());

    for (uint32_t i = 0; i < m_images.size(); i++) {
        m_image_views[i] = createImageView(m_images[i], m_image_format,
                                           VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }
}

VkImageView Swapchain::createImageView(VkImage image, VkFormat format,
                                       VkImageAspectFlags aspectFlags,
                                       uint32_t mipLevels) {
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = mipLevels;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(m_logical_device->getLogicalDevice(), &viewInfo,
                          nullptr, &imageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }

    return imageView;
}
