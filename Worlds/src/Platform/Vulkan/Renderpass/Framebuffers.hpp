#pragma once

#include "Platform/Vulkan/Image/Image2d.hpp"
#include "Platform/Vulkan/Renderpass/Swapchain.hpp"

namespace Worlds {
class RenderStage;
class Renderpass;
class ImageDepth;

class Framebuffers {
  public:
    Framebuffers(const glm::uvec2 &extent, const RenderStage &renderStage,
                 const Renderpass &renderpass, const Swapchain &swapchain,
                 const ImageDepth &depthStencil, VkSampleCountFlagBits samples);

    ~Framebuffers();

    Image2d *getAttachment(uint32_t index) const {
        return m_images[index].get();
    }

    const std::vector<Scope<Image2d>> &getImageAttachments() { return m_images; }
    const std::vector<VkFramebuffer> &getFramebuffers() { return m_framebuffers; }

  private:
  private:
    std::vector<Scope<Image2d>> m_images;
    std::vector<VkFramebuffer> m_framebuffers;
};

} // namespace Worlds
