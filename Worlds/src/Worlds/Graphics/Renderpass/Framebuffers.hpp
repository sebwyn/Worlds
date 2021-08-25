#pragma once

#include "Worlds/Graphics/Images/Image2d.hpp"
#include "Worlds/Graphics/Renderpass/Swapchain.hpp"

namespace Worlds {
class ImageDepth;
class Renderpass;
class RenderStage;

class Framebuffers {
  public:
    Framebuffers(const glm::uvec2 &extent, const RenderStage &renderStage,
                 const Renderpass &renderPass, const Swapchain &swapchain,
                 const ImageDepth &depthStencil,
                 VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT);
    ~Framebuffers();

    Image2d *GetAttachment(uint32_t index) const {
        return imageAttachments[index].get();
    }

    const std::vector<std::unique_ptr<Image2d>> &GetImageAttachments() const {
        return imageAttachments;
    }
    const std::vector<VkFramebuffer> &GetFramebuffers() const {
        return framebuffers;
    }

  private:
    std::vector<std::unique_ptr<Image2d>> imageAttachments;
    std::vector<VkFramebuffer> framebuffers;
};
} // namespace acid
