#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Worlds {
class RenderStage;

class SubpassDescription {
  public:
    SubpassDescription(VkPipelineBindPoint bindPoint,
                       std::vector<VkAttachmentReference> colorAttachments,
                       const std::optional<uint32_t> &depthAttachment)
        : m_colorAttachments(std::move(colorAttachments)) {

        m_subpassDescription.pipelineBindPoint = bindPoint;
        m_subpassDescription.colorAttachmentCount =
            static_cast<uint32_t>(this->m_colorAttachments.size());
        m_subpassDescription.pColorAttachments = this->m_colorAttachments.data();

        if (depthAttachment) {
            m_depthStencilAttachment.attachment = *depthAttachment;
            m_depthStencilAttachment.layout =
                VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            m_subpassDescription.pDepthStencilAttachment =
                &m_depthStencilAttachment;
        }
    }

    const VkSubpassDescription &getSubpassDescription() const {
        return m_subpassDescription;
    }

  private:
    VkSubpassDescription m_subpassDescription = {};
    std::vector<VkAttachmentReference> m_colorAttachments;
    VkAttachmentReference m_depthStencilAttachment = {};
};

class Renderpass {
  public:
    Renderpass(const RenderStage &renderStage, VkFormat surfaceFormat,
               VkFormat depthFormat,
               VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT);

    ~Renderpass();

    const VkRenderPass getRenderpass() const { return m_renderpass; }

  private:
  private:
    VkRenderPass m_renderpass;
};

} // namespace Worlds
