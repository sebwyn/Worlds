#include "Platform/Vulkan/Image/ImageDepth.hpp"

using namespace Worlds;

static const std::vector<VkFormat> TRY_FORMATS = {
    VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D32_SFLOAT,
    VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D16_UNORM_S8_UINT,
    VK_FORMAT_D16_UNORM};

ImageDepth::ImageDepth(const glm::uvec2 &extent, VkSampleCountFlagBits samples)
    : Image(
          VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, samples,
          VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
          VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT |
              VK_IMAGE_USAGE_SAMPLED_BIT,
          findSupportedFormats(TRY_FORMATS, VK_IMAGE_TILING_OPTIMAL,
                               VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT),
          1, 1, {extent.x, extent.y, 1}) {
    if (m_format == VK_FORMAT_UNDEFINED) {
        std::runtime_error("No depth stencil format could be selected");
    }

    VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    if (hasStencil(m_format))
        aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

    createImage(m_image, m_memory, m_extent, m_format, m_samples,
                VK_IMAGE_TILING_OPTIMAL, m_usage,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 1, 1, VK_IMAGE_TYPE_2D);
    createImageSampler(m_sampler, m_filter, m_address_mode, false, 1);
    createImageView(m_image, m_view, VK_IMAGE_VIEW_TYPE_2D, m_format,
                    VK_IMAGE_ASPECT_DEPTH_BIT, 1, 0, 1, 0);
    transitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED,
                          m_layout, aspectMask, 1, 0, 1, 0);
}
