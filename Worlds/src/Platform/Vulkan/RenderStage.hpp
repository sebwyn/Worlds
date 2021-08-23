#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Platform/Vulkan/Renderpass/Framebuffers.hpp"
#include "Platform/Vulkan/Renderpass/Renderpass.hpp"
#include "Platform/Vulkan/Descriptor/Descriptor.hpp"
#include "Platform/Vulkan/Image/ImageDepth.hpp"

namespace Worlds {

struct Color {
    float r, g, b, a;
};

class Attachment {
  public:
    enum class Type { Depth, Image, Swapchain };

    Attachment(uint32_t binding, std::string name, Type type, VkFormat format);

    uint32_t getBinding() const { return m_binding; }
    const std::string &getName() const { return m_name; }
    Type getType() const { return m_type; }
    VkFormat getFormat() const { return m_format; }
    Color getClearColor() const { return m_clear_color; }
    bool isMultisampled() const { return m_multisampled; }

  private:
  private:
    uint32_t m_binding;
    std::string m_name;
    Type m_type;
    VkFormat m_format;
    Color m_clear_color;
    bool m_multisampled;
};

class SubpassType {
  public:
    SubpassType(uint32_t binding, std::vector<uint32_t> attachmentBindings);

    uint32_t getBinding() const { return m_binding; }
    const std::vector<uint32_t> &getAttachmentBindings() const {
        return m_attachment_bindings;
    }

  private:
  private:
    uint32_t m_binding;
    std::vector<uint32_t> m_attachment_bindings;
};

class RenderArea {
  public:
    explicit RenderArea(const glm::uvec2 &extent = {},
                        const glm::ivec2 &offset = {})
        : m_extent(extent), m_offset(offset) {}

    bool operator==(const RenderArea &rhs) const {
        return m_extent == rhs.m_extent && m_offset == rhs.m_offset;
    }

    bool operator!=(const RenderArea &rhs) const { return !operator==(rhs); }

    const glm::uvec2 &getExtent() const { return m_extent; }
    void setExtent(const glm::uvec2 &extent) { m_extent = extent; }

    const glm::ivec2 &getOffset() const { return m_offset; }
    void setOffset(const glm::ivec2 &offset) { m_offset = offset; }

    /**
     * Gets the aspect ratio between the render stages width and height.
     * @return The aspect ratio.
     */
    float getAspectRatio() const { return m_aspect_ratio; }
    void setAspectRatio(float aspectRatio) { m_aspect_ratio = aspectRatio; }

  private:
    glm::uvec2 m_extent;
    glm::ivec2 m_offset;
    float m_aspect_ratio = 1.0f;
};

class Viewport {
  public:
    Viewport() = default;

    Viewport(glm::uvec2 size) : m_size(size) {}

    const glm::vec2 &getScale() const { return m_scale; }
    void setScale(const glm::vec2 &scale) { m_scale = scale; }

    const std::optional<glm::uvec2> &getSize() const { return m_size; }
    void setSize(const std::optional<glm::uvec2> &size) { m_size = size; }

    const glm::ivec2 &getOffset() const { return m_offset; }
    void setOffset(const glm::ivec2 &offset) { m_offset = offset; }

  private:
    glm::vec2 m_scale = {1.0f, 1.0f};
    std::optional<glm::uvec2> m_size;
    glm::ivec2 m_offset;
};

class RenderStage {
  public:
    RenderStage(std::vector<Attachment> images = {},
                std::vector<SubpassType> subpasses = {},
                const Viewport &viewport = Viewport());

    void onUpdate();

    void rebuild(const Swapchain &swapchain);

    std::optional<Attachment> getAttachment(const std::string &name) const;
    std::optional<Attachment> getAttachment(uint32_t binding) const;
    const Descriptor *getDescriptor(const std::string &name) const;
    const VkFramebuffer &
    getActiveFramebuffer(uint32_t activeSwapchainImage) const;

    const std::vector<Attachment> &getAttachments() const {
        return m_attachments;
    }
    const std::vector<SubpassType> &getSubpasses() const { return m_subpasses; }
    const Renderpass *getRenderpass() const { return m_renderpass.get(); }

  private:
  private:
    std::vector<Attachment> m_attachments;
    std::vector<SubpassType> m_subpasses;

    Viewport m_viewport;

    Scope<Renderpass> m_renderpass;
    Scope<ImageDepth> m_depth_stencil;
    Scope<Framebuffers> m_framebuffers;

    std::map<std::string, const Descriptor *> m_descriptors;

    std::vector<VkClearValue> m_clear_values;
    std::vector<uint32_t> m_subpass_attachment_count;
    std::vector<bool> m_subpass_multisampled;
    std::optional<Attachment> m_depth_attachment;
    std::optional<Attachment> m_swapchain_attachment;

    RenderArea m_render_area;
    bool m_out_of_date = false;
};

} // namespace Worlds
