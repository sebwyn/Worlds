#include "Platform/Vulkan/Renderpass/Renderpass.hpp"

#include "Platform/Vulkan/RenderStage.hpp"
#include "Platform/Vulkan/VKRenderAPI.hpp"

using namespace Worlds;

Renderpass::Renderpass(const RenderStage &renderStage, VkFormat surfaceFormat,
                       VkFormat depthFormat, VkSampleCountFlagBits samples) {

    std::vector<VkAttachmentDescription> attachmentDescriptions;

    for (const auto &attachment : renderStage.getAttachments()) {
        VkAttachmentDescription description;
        description.samples = samples;
        description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

        switch (attachment.getType()) {
        case (Attachment::Type::Image):
            description.format = attachment.getFormat();
            description.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            break;
        case (Attachment::Type::Depth):
            description.format = depthFormat;
            description.finalLayout =
                VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            break;
        case (Attachment::Type::Swapchain):
            description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            description.format = surfaceFormat;
            break;
        }

        attachmentDescriptions.emplace_back(description);
    }

    std::vector<Scope<SubpassDescription>> subpasses;
    std::vector<VkSubpassDependency> dependencies;

    for (const auto &subpassType : renderStage.getSubpasses()) {

        std::vector<VkAttachmentReference> colorAttachmentRefs;

        std::optional<uint32_t> depthAttachment;

        for (const auto &attachmentBinding :
             subpassType.getAttachmentBindings()) {
            auto attachment = renderStage.getAttachment(attachmentBinding);

            if (attachment.getType() == Attachment::Type::Depth) {
                depthAttachment = attachment.getBinding();
                continue;
            }

            VkAttachmentReference attachmentReference = {};
            attachmentReference.attachment = attachment.getBinding();
            attachmentReference.layout =
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            colorAttachmentRefs.emplace_back(attachmentReference);
        }

        subpasses.emplace_back(CreateScope<SubpassDescription>(
            VK_PIPELINE_BIND_POINT_GRAPHICS, colorAttachmentRefs,
            depthAttachment));

        VkSubpassDependency subpassDependency{};
        subpassDependency.srcStageMask =
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        subpassDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        subpassDependency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        subpassDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

        if (subpassType.getBinding() == renderStage.getSubpasses().size()) {
            subpassDependency.dstSubpass = VK_SUBPASS_EXTERNAL;
            subpassDependency.dstStageMask =
                VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
            subpassDependency.srcAccessMask =
                VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
                VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            subpassDependency.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        } else {
            subpassDependency.dstSubpass = subpassType.getBinding();
        }

        if (subpassType.getBinding() == 0) {
            subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
            subpassDependency.srcStageMask =
                VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
            subpassDependency.dstStageMask =
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            subpassDependency.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
            subpassDependency.dstAccessMask =
                VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
                VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        } else {
            subpassDependency.srcSubpass = subpassType.getBinding() - 1;
        }

        dependencies.emplace_back(subpassDependency);
    }

    std::vector<VkSubpassDescription> subpassDescriptions;
    subpassDescriptions.reserve(subpasses.size());

    for (const auto &subpass : subpasses) {
        subpassDescriptions.emplace_back(subpass->getSubpassDescription());
    }

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount =
        static_cast<uint32_t>(attachmentDescriptions.size());
    renderPassInfo.pAttachments = attachmentDescriptions.data();
    renderPassInfo.subpassCount =
        static_cast<uint32_t>(subpassDescriptions.size());
    renderPassInfo.pSubpasses = subpassDescriptions.data();
    renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
    renderPassInfo.pDependencies = dependencies.data();

    auto logicalDevice = VKRenderAPI::get()->getLogicalDevice();

    if (vkCreateRenderPass(logicalDevice->getLogicalDevice(), &renderPassInfo,
                           nullptr, &m_renderpass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}

Renderpass::~Renderpass() {
    auto logicalDevice = VKRenderAPI::get()->getLogicalDevice();

    vkDestroyRenderPass(logicalDevice->getLogicalDevice(), m_renderpass,
                        nullptr);
}
