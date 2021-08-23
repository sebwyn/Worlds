#pragma once

#include "Platform/Vulkan/Image/Image.hpp"

#include <glm/glm.hpp>

namespace Worlds {

class ImageDepth : public Image {
public:
    ImageDepth(const glm::uvec2 &extent, VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT);
};

}
