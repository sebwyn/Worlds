#pragma once

#include <map>

#include "Worlds/Graphics/Images/ImageDepth.hpp"
#include "Worlds/Graphics/Renderpass/Framebuffers.hpp"
#include "Worlds/Graphics/Renderpass/Renderpass.hpp"
#include "Worlds/Graphics/Renderpass/Swapchain.hpp"

namespace Worlds {
/**
 * @brief Class that represents an attachment in a renderpass.
 */
struct Color {
    float r, g, b, a;

    static Color Black;
};

class Attachment {
  public:
    enum class Type { Image, Depth, Swapchain };

    /**
     * Creates a new attachment that represents a object in the render pipeline.
     * @param binding The index the attachment is bound to in the renderpass.
     * @param name The unique name given to the object for all renderpasses.
     * @param type The attachment type this represents.
     * @param multisampled If this attachment is multisampled.
     * @param format The format that will be created (only applies to type
     * ATTACHMENT_IMAGE).
     * @param clearColour The colour to clear to before rendering to it.
     */
    Attachment(uint32_t binding, std::string name, Type type,
               bool multisampled = false,
               VkFormat format = VK_FORMAT_R8G8B8A8_UNORM,
               const Color &clearColour = Color::Black)
        : binding(binding), name(std::move(name)), type(type),
          multisampled(multisampled), format(format), clearColour(clearColour) {
    }

    uint32_t GetBinding() const { return binding; }
    const std::string &GetName() const { return name; }
    Type GetType() const { return type; }
    bool IsMultisampled() const { return multisampled; }
    VkFormat GetFormat() const { return format; }
    const Color &GetClearColour() const { return clearColour; }

  private:
    uint32_t binding;
    std::string name;
    Type type;
    bool multisampled;
    VkFormat format;
    Color clearColour;
};

class SubpassType {
  public:
    SubpassType(uint32_t binding, std::vector<uint32_t> attachmentBindings)
        : binding(binding), attachmentBindings(std::move(attachmentBindings)) {}

    uint32_t GetBinding() const { return binding; }
    const std::vector<uint32_t> &GetAttachmentBindings() const {
        return attachmentBindings;
    }

  private:
    uint32_t binding;
    std::vector<uint32_t> attachmentBindings;
};

class RenderArea {
  public:
    explicit RenderArea(const glm::uvec2 &extent = {},
                        const glm::ivec2 &offset = {})
        : extent(extent), offset(offset) {}

    bool operator==(const RenderArea &rhs) const {
        return extent == rhs.extent && offset == rhs.offset;
    }

    bool operator!=(const RenderArea &rhs) const { return !operator==(rhs); }

    const glm::uvec2 &GetExtent() const { return extent; }
    void SetExtent(const glm::uvec2 &extent) { this->extent = extent; }

    const glm::ivec2 &GetOffset() const { return offset; }
    void SetOffset(const glm::ivec2 &offset) { this->offset = offset; }

    /**
     * Gets the aspect ratio between the render stages width and height.
     * @return The aspect ratio.
     */
    float GetAspectRatio() const { return aspectRatio; }
    void SetAspectRatio(float aspectRatio) { this->aspectRatio = aspectRatio; }

  private:
    glm::uvec2 extent;
    glm::ivec2 offset;
    float aspectRatio = 1.0f;
};

class Viewport {
  public:
    Viewport() = default;

    explicit Viewport(const glm::uvec2 &size) : size(size) {}

    const glm::vec2 &GetScale() const { return scale; }
    void SetScale(const glm::vec2 &scale) { this->scale = scale; }

    const std::optional<glm::uvec2> &GetSize() const { return size; }
    void SetSize(const std::optional<glm::uvec2> &size) { this->size = size; }

    const glm::ivec2 &GetOffset() const { return offset; }
    void SetOffset(const glm::ivec2 &offset) { this->offset = offset; }

  private:
    glm::vec2 scale = {1.0f, 1.0f};
    std::optional<glm::uvec2> size;
    glm::ivec2 offset;
};

class RenderStage {
    friend class Graphics;

  public:
    explicit RenderStage(std::vector<Attachment> images = {},
                         std::vector<SubpassType> subpasses = {},
                         const Viewport &viewport = Viewport());

    void Update();
    void Rebuild(const Swapchain &swapchain);

    std::optional<Attachment> GetAttachment(const std::string &name) const;
    std::optional<Attachment> GetAttachment(uint32_t binding) const;

    const Descriptor *GetDescriptor(const std::string &name) const;
    const VkFramebuffer &
    GetActiveFramebuffer(uint32_t activeSwapchainImage) const;

    const std::vector<Attachment> &GetAttachments() const {
        return attachments;
    }
    const std::vector<SubpassType> &GetSubpasses() const { return subpasses; }

    Viewport &GetViewport() { return viewport; }
    void SetViewport(const Viewport &viewport) { this->viewport = viewport; }

    /**
     * Gets the render stage viewport.
     * @return The the render stage viewport.
     */
    const RenderArea &GetRenderArea() const { return renderArea; }

    /**
     * Gets if the width or height has changed between the last update and now.
     * @return If the width or height has changed.
     */
    bool IsOutOfDate() const { return outOfDate; }

    const Renderpass *GetRenderpass() const { return renderpass.get(); }
    const ImageDepth *GetDepthStencil() const { return depthStencil.get(); }
    const Framebuffers *GetFramebuffers() const { return framebuffers.get(); }
    const std::vector<VkClearValue> &GetClearValues() const {
        return clearValues;
    }
    uint32_t GetAttachmentCount(uint32_t subpass) const {
        return subpassAttachmentCount[subpass];
    }
    bool HasDepth() const { return depthAttachment.has_value(); }
    bool HasSwapchain() const { return swapchainAttachment.has_value(); }
    bool IsMultisampled(uint32_t subpass) const {
        return subpassMultisampled[subpass];
    }

  private:
    std::vector<Attachment> attachments;
    std::vector<SubpassType> subpasses;

    Viewport viewport;

    std::unique_ptr<Renderpass> renderpass;
    std::unique_ptr<ImageDepth> depthStencil;
    std::unique_ptr<Framebuffers> framebuffers;

    std::map<std::string, const Descriptor *> descriptors;

    std::vector<VkClearValue> clearValues;
    std::vector<uint32_t> subpassAttachmentCount;
    std::optional<Attachment> depthAttachment;
    std::optional<Attachment> swapchainAttachment;
    std::vector<bool> subpassMultisampled;

    RenderArea renderArea;
    bool outOfDate = false;
};
} // namespace Worlds
