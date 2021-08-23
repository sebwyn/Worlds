#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Worlds/Bitmap/Bitmap.hpp"
#include "Platform/Vulkan/Image/Image.hpp"

namespace Worlds {

class Image2d : public Image {
  public:
    Image2d(std::string filename, VkFilter filter = VK_FILTER_LINEAR,
            VkSamplerAddressMode addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            bool anisotropic = true, bool mipmap = true, bool load = true);

    Image2d(const glm::uvec2 extent, VkFormat format = VK_FORMAT_R8G8B8A8_UNORM,
            VkImageLayout layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            VkImageUsageFlags usage = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL |
                                      VK_IMAGE_USAGE_STORAGE_BIT,
            VkFilter filter = VK_FILTER_LINEAR,
            VkSamplerAddressMode addressMode =
                VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
            VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT,
            bool anisotropic = false, bool mipmap = false);

    bool IsAnisotropic() const { return m_anisotropic; }

  private:
    void load(Scope<Bitmap> loadBitmap = nullptr);
  private:
    std::string m_filename;

    bool m_anisotropic;
    bool m_mipmap;
    uint32_t m_components = 0;
};

} // namespace Worlds
