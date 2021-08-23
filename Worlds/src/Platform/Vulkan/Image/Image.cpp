#include "Platform/Vulkan/Image/Image.hpp"

#include "Platform/Vulkan/Buffer/Buffer.hpp"
#include "Platform/Vulkan/VKRenderAPI.hpp"

static constexpr float ANISOTROPY = 16.0f;

using namespace Worlds;

Image::Image(VkFilter filter, VkSamplerAddressMode addressMode,
             VkSampleCountFlagBits samples, VkImageLayout layout,
             VkImageUsageFlags usage, VkFormat format, uint32_t mipLevels,
             uint32_t arrayLayers, const VkExtent3D &extent)
    : m_extent(extent), m_samples(samples), m_usage(usage), m_format(format),
      m_mip_levels(mipLevels), m_array_layers(arrayLayers), m_filter(filter),
      m_address_mode(addressMode), m_layout(layout) {}

Image::~Image() {
    auto logicalDevice = VKRenderAPI::get()->getLogicalDevice();

    vkDestroyImageView(logicalDevice->getLogicalDevice(), m_view, nullptr);
    vkDestroySampler(logicalDevice->getLogicalDevice(), m_sampler, nullptr);
    vkFreeMemory(logicalDevice->getLogicalDevice(), m_memory, nullptr);
    vkDestroyImage(logicalDevice->getLogicalDevice(), m_image, nullptr);
}

uint32_t Image::getMipLevels(const VkExtent3D &extent) {
    return static_cast<uint32_t>(
        std::floor(std::log2(
            std::max(extent.width, std::max(extent.height, extent.depth)))) +
        1);
}

VkFormat Image::findSupportedFormats(const std::vector<VkFormat> &candidates,
                                     VkImageTiling tiling,
                                     VkFormatFeatureFlags features) {
    auto physicalDevice = VKRenderAPI::get()->getPhysicalDevice();

    for (const auto &format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(physicalDevice->getPhysicalDevice(),
                                            format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR &&
            (props.linearTilingFeatures & features) == features)
            return format;
        if (tiling == VK_IMAGE_TILING_LINEAR &&
            (props.optimalTilingFeatures & features) == features)
            return format;
    }

    return VK_FORMAT_UNDEFINED;
}

bool Image::hasStencil(VkFormat format) {
    static const std::vector<VkFormat> STENCIL_FORMATS = {
        VK_FORMAT_S8_UINT, VK_FORMAT_D16_UNORM_S8_UINT,
        VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D32_SFLOAT_S8_UINT};
    return std::find(STENCIL_FORMATS.begin(), STENCIL_FORMATS.end(), format) !=
           std::end(STENCIL_FORMATS);
}

void Image::createImage(VkImage &image, VkDeviceMemory &memory,
                        const VkExtent3D &extent, VkFormat format,
                        VkSampleCountFlagBits samples, VkImageTiling tiling,
                        VkImageUsageFlags usage,
                        VkMemoryPropertyFlags properties, uint32_t mipLevels,
                        uint32_t arrayLayers, VkImageType type) {
    auto logicalDevice = VKRenderAPI::get()->getLogicalDevice();

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = type;
    imageInfo.format = format;
    imageInfo.extent = extent;
    imageInfo.mipLevels = mipLevels;
    imageInfo.arrayLayers = arrayLayers;
    imageInfo.samples = samples;
    imageInfo.tiling = tiling;
    imageInfo.usage = usage;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    if (vkCreateImage(logicalDevice->getLogicalDevice(), &imageInfo, nullptr,
                      &image) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(logicalDevice->getLogicalDevice(), image,
                                 &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex =
        Buffer::findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(logicalDevice->getLogicalDevice(), &allocInfo, nullptr,
                         &memory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(logicalDevice->getLogicalDevice(), image, memory, 0);
}

void Image::createImageSampler(VkSampler &sampler, VkFilter filter,
                               VkSamplerAddressMode addressMode,
                               bool anisotropic, uint32_t mipLevels) {
    auto physicalDevice = VKRenderAPI::get()->getPhysicalDevice();
    auto logicalDevice = VKRenderAPI::get()->getLogicalDevice();

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = filter;
    samplerInfo.minFilter = filter;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.addressModeU = addressMode;
    samplerInfo.addressModeV = addressMode;
    samplerInfo.addressModeW = addressMode;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.anisotropyEnable = static_cast<VkBool32>(anisotropic);
    samplerInfo.maxAnisotropy =
        (anisotropic && logicalDevice->getEnabledFeatures().samplerAnisotropy)
            ? std::min(
                  ANISOTROPY,
                  physicalDevice->getProperties().limits.maxSamplerAnisotropy)
            : 1.0;

    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = static_cast<float>(mipLevels);
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_WHITE;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    if (vkCreateSampler(logicalDevice->getLogicalDevice(), &samplerInfo,
                        nullptr, &sampler) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture sampler!");
    }
}

void Image::createImageView(const VkImage &image, VkImageView &imageView,
                            VkImageViewType type, VkFormat format,
                            VkImageAspectFlags imageAspect, uint32_t mipLevels,
                            uint32_t baseMipLevel, uint32_t layerCount,
                            uint32_t baseArrayLayer) {
    auto logicalDevice = VKRenderAPI::get()->getLogicalDevice();

    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = type;
    viewInfo.format = format;
    viewInfo.components = {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G,
                           VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A};
    viewInfo.subresourceRange.aspectMask = imageAspect;
    viewInfo.subresourceRange.baseMipLevel = baseMipLevel;
    viewInfo.subresourceRange.levelCount = mipLevels;
    viewInfo.subresourceRange.baseArrayLayer = baseArrayLayer;
    viewInfo.subresourceRange.layerCount = layerCount;

    if (vkCreateImageView(logicalDevice->getLogicalDevice(), &viewInfo, nullptr,
                          &imageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }
}

void Image::createMipMaps(const VkImage &image, const VkExtent3D &extent,
                          VkFormat format, VkImageLayout dstImageLayout,
                          uint32_t mipLevels, uint32_t baseArrayLayer,
                          uint32_t layerCount) {
    auto physicalDevice = VKRenderAPI::get()->getPhysicalDevice();

    // Check if image format supports linear blitting
    VkFormatProperties formatProperties;
    vkGetPhysicalDeviceFormatProperties(physicalDevice->getPhysicalDevice(),
                                        format, &formatProperties);

    if (!(formatProperties.optimalTilingFeatures &
          VK_FORMAT_FEATURE_BLIT_SRC_BIT)) {
        throw std::runtime_error(
            "texture image format does not support blitting!");
    }
    if (!(formatProperties.optimalTilingFeatures &
          VK_FORMAT_FEATURE_BLIT_DST_BIT)) {
        throw std::runtime_error(
            "texture image format does not support blitting!");
    }

    CommandBuffer commandBuffer;

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseArrayLayer = baseArrayLayer;
    barrier.subresourceRange.layerCount = layerCount;

    for (uint32_t i = 1; i < mipLevels; i++) {
        barrier.subresourceRange.baseMipLevel = i - 1;
        barrier.subresourceRange.levelCount = 1;
        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                             VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0,
                             nullptr, 1, &barrier);

        VkImageBlit blit{};
        blit.srcOffsets[1] = {int32_t(extent.width >> (i - 1)),
                              int32_t(extent.height >> (i - 1)), 1};
        blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.srcSubresource.mipLevel = i - 1;
        blit.srcSubresource.baseArrayLayer = baseArrayLayer;
        blit.srcSubresource.layerCount = layerCount;
        blit.dstOffsets[1] = {int32_t(extent.width >> i),
                              int32_t(extent.height >> i), 1};
        blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.dstSubresource.mipLevel = i;
        blit.dstSubresource.baseArrayLayer = baseArrayLayer;
        blit.dstSubresource.layerCount = layerCount;
        vkCmdBlitImage(
            commandBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout = dstImageLayout;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                             VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0,
                             nullptr, 0, nullptr, 1, &barrier);
    }

    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = dstImageLayout;
    barrier.subresourceRange.baseMipLevel = mipLevels - 1;
    vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                         VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr,
                         0, nullptr, 1, &barrier);

    commandBuffer.submitIdle();
}

void transitionImageLayout(const VkImage &image, VkFormat format,
                           VkImageLayout srcImageLayout,
                           VkImageLayout dstImageLayout,
                           VkImageAspectFlags aspect, uint32_t mipLevels,
                           uint32_t baseMipLevel, uint32_t layerCount,
                           uint32_t baseArrayLayer) {

    CommandBuffer commandBuffer;

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = srcImageLayout;
    barrier.newLayout = dstImageLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = aspect;
    barrier.subresourceRange.baseMipLevel = baseMipLevel;
    barrier.subresourceRange.levelCount = mipLevels;
    barrier.subresourceRange.baseArrayLayer = baseArrayLayer;
    barrier.subresourceRange.layerCount = layerCount;

    switch (srcImageLayout) {
    case VK_IMAGE_LAYOUT_UNDEFINED:
        barrier.srcAccessMask = 0;
        break;
    case VK_IMAGE_LAYOUT_PREINITIALIZED:
        barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
        break;
    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
        barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        break;
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
        barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        break;
    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        break;
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        break;
    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
        barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
        break;
    default:
        break;
    }

    switch (dstImageLayout) {
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        break;
    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        break;
    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
        barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        break;
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
        barrier.dstAccessMask = barrier.dstAccessMask |
                                VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        break;
    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
        if (barrier.srcAccessMask == 0) {
            barrier.srcAccessMask =
                VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
        }
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        break;
    default:
        break;
    }
    vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                         VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0,
                         nullptr, 1, &barrier);

    commandBuffer.submitIdle();
}

void Image::copyBufferToImage(const VkBuffer &buffer, const VkImage &image,
                              const VkExtent3D &extent, uint32_t layerCount,
                              uint32_t baseArrayLayer) {

    CommandBuffer commandBuffer;

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = baseArrayLayer;
    region.imageSubresource.layerCount = layerCount;
    region.imageOffset = {0, 0, 0};
    region.imageExtent = extent;

    vkCmdCopyBufferToImage(commandBuffer, buffer, image,
                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    commandBuffer.submitIdle();
}
