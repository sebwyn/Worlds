#include "Platform/Vulkan/Image/Image2d.hpp"

#include "Platform/Vulkan/Buffer/Buffer.hpp"

using namespace Worlds;

Image2d::Image2d(std::string filename, VkFilter filter,
                 VkSamplerAddressMode addressMode, bool anisotropic,
                 bool mipmap, bool load)
    : Image(filter, addressMode, VK_SAMPLE_COUNT_1_BIT,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_FORMAT_R8G8B8A8_UNORM, 1, 1, {0, 0, 1}),
      m_filename(std::move(filename)), m_anisotropic(anisotropic),
      m_mipmap(mipmap) {

    if (load)
        this->load();
}

Image2d::Image2d(const glm::uvec2 extent, VkFormat format, VkImageLayout layout,
                 VkImageUsageFlags usage, VkFilter filter,
                 VkSamplerAddressMode addressMode,
                 VkSampleCountFlagBits samples, bool anisotropic, bool mipmap)
    : Image(filter, addressMode, samples, layout,
            usage | VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            format, 1, 1, {extent.x, extent.y, 1}),
      m_anisotropic(anisotropic), m_mipmap(mipmap), m_components(4) {
    this->load();
}

void Image2d::load(Scope<Bitmap> loadBitmap) {
    if (!(m_filename.length() == 0) && !loadBitmap) {
        loadBitmap = CreateScope<Bitmap>(m_filename);
        m_extent = {loadBitmap->getSize().y, loadBitmap->getSize().y, 1};
        m_components = loadBitmap->getBytesPerPixel();
    }

    if (m_extent.width == 0 || m_extent.height == 0)
        return;

    m_mip_levels = m_mipmap ? getMipLevels(m_extent) : 1;

    createImage(m_image, m_memory, m_extent, m_format, m_samples,
                VK_IMAGE_TILING_OPTIMAL, m_usage,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_mip_levels,
                m_array_layers, VK_IMAGE_TYPE_2D);

    createImageSampler(m_sampler, m_filter, m_address_mode, m_anisotropic,
                       m_mip_levels);
    createImageView(m_image, m_view, VK_IMAGE_VIEW_TYPE_2D, m_format,
                    VK_IMAGE_ASPECT_COLOR_BIT, m_mip_levels, 0, m_array_layers,
                    0);
    if (loadBitmap || m_mipmap) {
        transitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED,
                              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                              VK_IMAGE_ASPECT_COLOR_BIT, m_mip_levels, 0,
                              m_array_layers, 0);
    }

    if (loadBitmap) {
        Buffer bufferStaging(loadBitmap->getLength(),
                             VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        uint8_t *data;
        bufferStaging.mapMemory(reinterpret_cast<void **>(&data));
        std::memcpy(data, loadBitmap->getData().get(), bufferStaging.getSize());
        bufferStaging.unmapMemory();

        copyBufferToImage(bufferStaging.getBuffer(), m_image, m_extent,
                          m_array_layers, 0);
    }

    if (m_mipmap) {
        createMipMaps(m_image, m_extent, m_format, m_layout, m_mip_levels, 0,
                      m_array_layers);
    } else {
        transitionImageLayout(m_image, m_format, VK_IMAGE_LAYOUT_UNDEFINED,
                              m_layout, VK_IMAGE_ASPECT_COLOR_BIT, m_mip_levels,
                              0, m_array_layers, 0);
    }
}
