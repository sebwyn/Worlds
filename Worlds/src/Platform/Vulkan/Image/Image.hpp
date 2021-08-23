#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Platform/Vulkan/Command/CommandBuffer.hpp"

namespace Worlds {

class Image {
  public:
    Image(VkFilter filter, VkSamplerAddressMode addressMode,
          VkSampleCountFlagBits samples, VkImageLayout layout,
          VkImageUsageFlags usage, VkFormat format, uint32_t mipLevels,
          uint32_t arrayLayers, const VkExtent3D &extent);

    ~Image();

    const VkImageView getView() const { return m_view; }

    static uint32_t getMipLevels(const VkExtent3D &extent);
    static VkFormat
    findSupportedFormats(const std::vector<VkFormat> &candidates,
                         VkImageTiling tiling, VkFormatFeatureFlags features);

    static bool hasStencil(VkFormat format);

    static void createImage(VkImage &image, VkDeviceMemory &memory,
                            const VkExtent3D &extent, VkFormat format,
                            VkSampleCountFlagBits samples, VkImageTiling tiling,
                            VkImageUsageFlags usage,
                            VkMemoryPropertyFlags properties,
                            uint32_t mipLevels, uint32_t arrayLayers,
                            VkImageType type);

    static void createImageSampler(VkSampler &sampler, VkFilter filter,
                                   VkSamplerAddressMode addressMode,
                                   bool anisotropic, uint32_t mipLevels);

    static void createImageView(const VkImage &image, VkImageView &imageView,
                                VkImageViewType type, VkFormat format,
                                VkImageAspectFlags imageAspect,
                                uint32_t mipLevels, uint32_t baseMipLevel,
                                uint32_t layerCount, uint32_t baseArrayLayer);

    static void createMipMaps(const VkImage &image, const VkExtent3D &extent,
                              VkFormat format, VkImageLayout dstImageLayout,
                              uint32_t mipLevels, uint32_t baseArrayLayer,
                              uint32_t layerCount);

    static void transitionImageLayout(const VkImage &image, VkFormat format,
                                      VkImageLayout srcImageLayout,
                                      VkImageLayout dstImageLayout,
                                      VkImageAspectFlags aspect,
                                      uint32_t mipLevels, uint32_t baseMipLevel,
                                      uint32_t layerCount,
                                      uint32_t baseArrayLayer);

    static void copyBufferToImage(const VkBuffer &buffer, const VkImage &image,
                                  const VkExtent3D &extent, uint32_t layerCount,
                                  uint32_t baseArrayLayer);

  private:
  protected:
    VkExtent3D m_extent;
    VkSampleCountFlagBits m_samples;
    VkImageUsageFlags m_usage;
    VkFormat m_format;
    uint32_t m_mip_levels;
    uint32_t m_array_layers;
    VkFilter m_filter;
    VkSamplerAddressMode m_address_mode;
    VkImageLayout m_layout;

    VkDeviceMemory m_memory = VK_NULL_HANDLE;
    VkImage m_image = VK_NULL_HANDLE;
    VkImageView m_view = VK_NULL_HANDLE;
    VkSampler m_sampler = VK_NULL_HANDLE;
};

} // namespace Worlds
