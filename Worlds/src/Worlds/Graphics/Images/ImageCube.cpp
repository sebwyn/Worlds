#include "Worlds/Graphics/Images/ImageCube.hpp"

#include <cstring>

#include "Worlds/Bitmaps/Bitmap.hpp"
#include "Worlds/Graphics/Buffers/Buffer.hpp"
#include "Worlds/Graphics/Graphics.hpp"
#include "Worlds/Graphics/Images/Image.hpp"

namespace Worlds {
ImageCube::ImageCube(std::filesystem::path filename, std::string fileSuffix,
                     VkFilter filter, VkSamplerAddressMode addressMode,
                     bool anisotropic, bool mipmap, bool load)
    : Image(filter, addressMode, VK_SAMPLE_COUNT_1_BIT,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_FORMAT_R8G8B8A8_UNORM, 1, 6, {0, 0, 1}),
      filename(std::move(filename)), fileSuffix(std::move(fileSuffix)),
      anisotropic(anisotropic), mipmap(mipmap) {
    if (load) {
        ImageCube::Load();
    }
}

ImageCube::ImageCube(const glm::uvec2 &extent, VkFormat format,
                     VkImageLayout layout, VkImageUsageFlags usage,
                     VkFilter filter, VkSamplerAddressMode addressMode,
                     VkSampleCountFlagBits samples, bool anisotropic,
                     bool mipmap)
    : Image(filter, addressMode, samples, layout,
            usage | VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            format, 1, 6, {extent.x, extent.y, 1}),
      anisotropic(anisotropic), mipmap(mipmap), components(4) {
    ImageCube::Load();
}

ImageCube::ImageCube(std::unique_ptr<Bitmap> &&bitmap, VkFormat format,
                     VkImageLayout layout, VkImageUsageFlags usage,
                     VkFilter filter, VkSamplerAddressMode addressMode,
                     VkSampleCountFlagBits samples, bool anisotropic,
                     bool mipmap)
    : Image(filter, addressMode, samples, layout,
            usage | VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            format, 1, 6, {bitmap->GetSize().x, bitmap->GetSize().y, 1}),
      anisotropic(anisotropic), mipmap(mipmap),
      components(bitmap->GetBytesPerPixel()) {
    ImageCube::Load(std::move(bitmap));
}

ImageCube::~ImageCube() {
    auto logicalDevice = Graphics::Get()->GetLogicalDevice();

    vkDestroyImageView(*logicalDevice, view, nullptr);
    vkDestroySampler(*logicalDevice, sampler, nullptr);
    vkFreeMemory(*logicalDevice, memory, nullptr);
    vkDestroyImage(*logicalDevice, image, nullptr);
}

std::unique_ptr<Bitmap> ImageCube::GetBitmap(uint32_t mipLevel) const {
    auto size = glm::uvec2(extent.width, extent.height) >> mipLevel;
    auto sizeSide = size.x * size.y * components;
    auto bitmap = std::make_unique<Bitmap>(
        glm::uvec2(size.x, size.y * arrayLayers), components);
    auto offset = bitmap->GetData().get();

    for (uint32_t i = 0; i < 6; i++) {
        auto bitmapSide = Image::GetBitmap(mipLevel, i);
        std::memcpy(offset, bitmapSide->GetData().get(), sizeSide);
        offset += sizeSide;
    }

    return bitmap;
}

void ImageCube::SetPixels(const uint8_t *pixels, uint32_t layerCount,
                          uint32_t baseArrayLayer) {
    Buffer bufferStaging(extent.width * extent.height * components *
                             arrayLayers,
                         VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                             VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    void *data;
    bufferStaging.MapMemory(&data);
    memcpy(data, pixels, bufferStaging.GetSize());
    bufferStaging.UnmapMemory();

    CopyBufferToImage(bufferStaging.GetBuffer(), image, extent, layerCount,
                      baseArrayLayer);
}

void ImageCube::Load(std::unique_ptr<Bitmap> loadBitmap) {
    if (!filename.empty() && !loadBitmap) {
        uint8_t *offset = nullptr;

        for (const auto &side : fileSides) {
            Bitmap bitmapSide(filename / (side + fileSuffix));
            auto lengthSide = bitmapSide.GetLength();

            if (!loadBitmap) {
                loadBitmap = std::make_unique<Bitmap>(
                    std::make_unique<uint8_t[]>(lengthSide * arrayLayers),
                    bitmapSide.GetSize(), bitmapSide.GetBytesPerPixel());
                offset = loadBitmap->GetData().get();
            }

            std::memcpy(offset, bitmapSide.GetData().get(), lengthSide);
            offset += lengthSide;
        }

        extent = {loadBitmap->GetSize().y, loadBitmap->GetSize().y, 1};
        components = loadBitmap->GetBytesPerPixel();
    }

    if (extent.width == 0 || extent.height == 0) {
        return;
    }

    mipLevels = mipmap ? GetMipLevels(extent) : 1;

    CreateImage(image, memory, extent, format, samples, VK_IMAGE_TILING_OPTIMAL,
                usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mipLevels,
                arrayLayers, VK_IMAGE_TYPE_2D);
    CreateImageSampler(sampler, filter, addressMode, anisotropic, mipLevels);
    CreateImageView(image, view, VK_IMAGE_VIEW_TYPE_CUBE, format,
                    VK_IMAGE_ASPECT_COLOR_BIT, mipLevels, 0, arrayLayers, 0);

    if (loadBitmap || mipmap) {
        TransitionImageLayout(image, format, VK_IMAGE_LAYOUT_UNDEFINED,
                              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                              VK_IMAGE_ASPECT_COLOR_BIT, mipLevels, 0,
                              arrayLayers, 0);
    }

    if (loadBitmap) {
        Buffer bufferStaging(loadBitmap->GetLength() * arrayLayers,
                             VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        uint8_t *data;
        bufferStaging.MapMemory(reinterpret_cast<void **>(&data));
        std::memcpy(data, loadBitmap->GetData().get(), bufferStaging.GetSize());
        bufferStaging.UnmapMemory();

        CopyBufferToImage(bufferStaging.GetBuffer(), image, extent, arrayLayers,
                          0);
    }

    if (mipmap) {
        CreateMipmaps(image, extent, format, layout, mipLevels, 0, arrayLayers);
    } else if (loadBitmap) {
        TransitionImageLayout(
            image, format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, layout,
            VK_IMAGE_ASPECT_COLOR_BIT, mipLevels, 0, arrayLayers, 0);
    } else {
        TransitionImageLayout(image, format, VK_IMAGE_LAYOUT_UNDEFINED, layout,
                              VK_IMAGE_ASPECT_COLOR_BIT, mipLevels, 0,
                              arrayLayers, 0);
    }
}
} // namespace Worlds
