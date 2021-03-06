#pragma once

#include "Worlds/Devices/Instance.hpp"
#include "Worlds/Devices/LogicalDevice.hpp"
#include "Worlds/Devices/PhysicalDevice.hpp"
#include "Worlds/Devices/Surface.hpp"
#include "Worlds/Graphics/Commands/CommandBuffer.hpp"
#include "Worlds/Graphics/Commands/CommandPool.hpp"
#include "Worlds/Graphics/Renderer.hpp"

#include "Worlds/Core/Window.hpp"
#include "Worlds/Core/Module.hpp"

namespace Worlds {
/**
 * @brief Module that manages the Vulkan instance, Surface, Window and the
 * renderpass structure.
 */
class Graphics : public Module::Registrar<Graphics> {
    inline static const bool Registered = Register(Stage::Render, Requires<WindowAPI>());
  public:
    Graphics();

    ~Graphics();

    void Update();

    static Graphics *Get() { return s_instance; }

    static std::string StringifyResultVk(VkResult result);
    static void CheckVk(VkResult result);

    /**
     * Takes a screenshot of the current image of the display and saves it into
     * a image file.
     * @param filename The file to save the screenshot as.
     */
    void CaptureScreenshot(const std::filesystem::path &filename) const;

    const std::shared_ptr<CommandPool> &GetCommandPool(
        const std::thread::id &threadId = std::this_thread::get_id());

    /**
     * Gets the current renderer.
     * @return The renderer.
     */
    Renderer *GetRenderer() const { return renderer.get(); }

    /**
     * Sets the current renderer to a new renderer.
     * @param renderer The new renderer.
     */
    void SetRenderer(std::unique_ptr<Renderer> &&renderer) {
        this->renderer = std::move(renderer);
    }

    RenderStage *GetRenderStage(uint32_t index) const;

    const Descriptor *GetAttachment(const std::string &name) const;
    const Swapchain *GetSwapchain() const { return swapchain.get(); }
    const VkPipelineCache &GetPipelineCache() const { return pipelineCache; }
    void SetFramebufferResized() { framebufferResized = true; }
    const PhysicalDevice *GetPhysicalDevice() const {
        return physicalDevice.get();
    }
    const Surface *GetSurface() const { return surface.get(); }
    const LogicalDevice *GetLogicalDevice() const {
        return logicalDevice.get();
    }

  private:
    void CreatePipelineCache();
    void ResetRenderStages();
    void RecreateSwapchain();
    void RecreateCommandBuffers();
    void RecreatePass(RenderStage &renderStage);
    void RecreateAttachmentsMap();
    bool StartRenderpass(RenderStage &renderStage);
    void EndRenderpass(RenderStage &renderStage);

    static Graphics *s_instance;

    std::unique_ptr<Renderer> renderer;
    std::map<std::string, const Descriptor *> attachments;
    std::unique_ptr<Swapchain> swapchain;

    std::map<std::thread::id, std::shared_ptr<CommandPool>> commandPools;
    /// Timer used to remove unused command pools.
    // ElapsedTime elapsedPurge;

    VkPipelineCache pipelineCache = VK_NULL_HANDLE;
    std::vector<VkSemaphore> presentCompletes;
    std::vector<VkSemaphore> renderCompletes;
    std::vector<VkFence> flightFences;
    std::size_t currentFrame = 0;
    bool framebufferResized = false;

    std::vector<std::unique_ptr<CommandBuffer>> commandBuffers;

    std::unique_ptr<Instance> instance;
    std::unique_ptr<PhysicalDevice> physicalDevice;
    std::unique_ptr<Surface> surface;
    std::unique_ptr<LogicalDevice> logicalDevice;
};
} // namespace Worlds
