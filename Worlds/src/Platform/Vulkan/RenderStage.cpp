#include "Platform/Vulkan/RenderStage.hpp"

#include "Platform/Vulkan/VKRenderAPI.hpp"

namespace Worlds {

RenderStage::RenderStage(std::vector<Attachment> images,
                         std::vector<SubpassType> subpasses,
                         const Viewport &viewport)
    : m_attachments(std::move(images)), m_subpasses(std::move(subpasses)),
      m_viewport(viewport), m_subpass_attachment_count(m_subpasses.size()),
      m_subpass_multisampled(m_subpasses.size()) {
    for (const auto &image : m_attachments) {
        VkClearValue clearValue = {};

        switch (image.getType()) {
        case Attachment::Type::Image:
            clearValue.color = {
                {image.getClearColor().r, image.getClearColor().g,
                 image.getClearColor().b, image.getClearColor().a}};

            for (const auto &subpass : m_subpasses) {
                if (auto subpassBindings = subpass.getAttachmentBindings();
                    std::find(subpassBindings.begin(), subpassBindings.end(),
                              image.getBinding()) != subpassBindings.end()) {
                    m_subpass_attachment_count[subpass.getBinding()]++;

                    if (image.isMultisampled())
                        m_subpass_multisampled[subpass.getBinding()] = true;
                }
            }

            break;
        case Attachment::Type::Depth:
            clearValue.depthStencil = {1.0f, 0};
            m_depth_attachment = image;
            break;
        case Attachment::Type::Swapchain:
            clearValue.color = {
                {image.getClearColor().r, image.getClearColor().g,
                 image.getClearColor().b, image.getClearColor().a}};
            m_swapchain_attachment = image;
            break;
        }

        m_clear_values.emplace_back(clearValue);
    }
}

void RenderStage::onUpdate() {
    auto lastRenderArea = m_render_area;

    m_render_area.setOffset(m_viewport.getOffset());

    if (m_viewport.getSize())
        m_render_area.setExtent(m_viewport.getScale() *
                                glm::vec2(*m_viewport.getSize()));
    else
        m_render_area.setExtent(m_viewport.getScale() *
                                Window::Get()->GetSize());

    m_render_area.setAspectRatio(
        static_cast<float>(m_render_area.getExtent().x) /
        static_cast<float>(m_render_area.getExtent().y));
    m_render_area.setExtent(glm::ivec2(m_render_area.getExtent()) +
                            m_render_area.getOffset());

    m_out_of_date = m_render_area != lastRenderArea;
}

void RenderStage::rebuild(const Swapchain &swapchain) {
#if defined(ACID_DEBUG)
    auto debugStart = Time::Now();
#endif

    onUpdate();

    auto physicalDevice = VKRenderAPI::get()->getPhysicalDevice();
    auto surface = VKRenderAPI::get()->getSurface();

    auto msaaSamples = physicalDevice->getSampleCount();

    if (m_depth_attachment)
        m_depth_stencil = CreateScope<ImageDepth>(
            m_render_area.getExtent(), m_depth_attachment->isMultisampled()
                                           ? msaaSamples
                                           : VK_SAMPLE_COUNT_1_BIT);

    if (!m_renderpass)
        m_renderpass = std::make_unique<Renderpass>(
            *this,
            m_depth_stencil ? m_depth_stencil->getFormat()
                            : VK_FORMAT_UNDEFINED,
            surface->getFormat().format, msaaSamples);

    m_framebuffers = std::make_unique<Framebuffers>(
        m_render_area.getExtent(), *this, *m_renderpass, swapchain,
        *m_depth_stencil, msaaSamples);
    m_out_of_date = false;

    m_descriptors.clear();

    for (const auto &image : m_attachments) {
        if (image.getType() == Attachment::Type::Depth)
            m_descriptors.emplace(image.getName(), m_depth_stencil.get());
        else
            m_descriptors.emplace(
                image.getName(),
                m_framebuffers->getAttachment(image.getBinding()));
    }

#if defined(ACID_DEBUG)
    Log::Out("Render Stage created in ",
             (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}

std::optional<Attachment>
RenderStage::getAttachment(const std::string &name) const {
    auto it = std::find_if(
        m_attachments.begin(), m_attachments.end(),
        [name](const Attachment &a) { return a.getName() == name; });

    if (it != m_attachments.end())
        return *it;
    return std::nullopt;
}

std::optional<Attachment> RenderStage::getAttachment(uint32_t binding) const {
    auto it = std::find_if(
        m_attachments.begin(), m_attachments.end(),
        [binding](const Attachment &a) { return a.getBinding() == binding; });

    if (it != m_attachments.end())
        return *it;
    return std::nullopt;
}

const Descriptor *RenderStage::getDescriptor(const std::string &name) const {
    auto it = m_descriptors.find(name);

    if (it != m_descriptors.end())
        return it->second;
    return nullptr;
}

const VkFramebuffer &
RenderStage::getActiveFramebuffer(uint32_t activeSwapchainImage) const {
    if (activeSwapchainImage > m_framebuffers->getFramebuffers().size())
        return m_framebuffers->getFramebuffers().at(0);

    return m_framebuffers->getFramebuffers().at(activeSwapchainImage);
}

} // namespace Worlds
