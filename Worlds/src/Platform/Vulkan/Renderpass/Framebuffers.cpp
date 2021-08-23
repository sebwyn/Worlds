#include "Platform/Vulkan/Renderpass/Framebuffers.hpp"

#include "Platform/Vulkan/Image/ImageDepth.hpp"
#include "Platform/Vulkan/RenderStage.hpp"
#include "Platform/Vulkan/Renderpass/Renderpass.hpp"
#include "Platform/Vulkan/VKRenderAPI.hpp"

using namespace Worlds;

Framebuffers::Framebuffers(const glm::uvec2 &extent,
                           const RenderStage &renderStage,
                           const Renderpass &renderpass,
                           const Swapchain &swapchain,
                           const ImageDepth &depthStencil,
                           VkSampleCountFlagBits samples) {
    auto logicalDevice = VKRenderAPI::get()->getLogicalDevice();

    for (const auto &attachment : renderStage.getAttachments()) {
        switch (attachment.getType()) {
        case (Attachment::Type::Image):
            m_images.emplace_back(CreateScope<Image2d>(
                extent, attachment.getFormat(),
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                    VK_IMAGE_USAGE_STORAGE_BIT,
                VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
                samples));
            break;
        case (Attachment::Type::Depth):
        case (Attachment::Type::Swapchain):
            m_images.emplace_back(nullptr);
            break;
        }
    }

    m_framebuffers.resize(swapchain.getSize());

    for (size_t i = 0; i < swapchain.getSize(); i++) {
        std::vector<VkImageView> attachments;

        for (const auto &attachment : renderStage.getAttachments()) {
            switch (attachment.getType()) {
            case (Attachment::Type::Image):
                attachments.emplace_back(
                    getAttachment(attachment.getBinding())->getView());
                break;
            case (Attachment::Type::Depth):
                attachments.emplace_back(depthStencil.getView());
                break;
            case (Attachment::Type::Swapchain):
                attachments.emplace_back(swapchain.getImageViews().at(i));
                break;
            }
        }

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass =
            renderStage.getRenderpass()->getRenderpass();
        framebufferInfo.attachmentCount =
            static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = extent.x;
        framebufferInfo.height = extent.y;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(logicalDevice->getLogicalDevice(),
                                &framebufferInfo, nullptr,
                                &m_framebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

Framebuffers::~Framebuffers() {
    auto logicalDevice = VKRenderAPI::get()->getLogicalDevice();

    for (const auto &framebuffer : m_framebuffers) {
        vkDestroyFramebuffer(logicalDevice->getLogicalDevice(), framebuffer,
                             nullptr);
    }
}
