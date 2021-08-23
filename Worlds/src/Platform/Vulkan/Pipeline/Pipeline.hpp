#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Platform/Vulkan/Command/CommandBuffer.hpp"
#include "Platform/Vulkan/Pipeline/Shader.hpp"

namespace Worlds {

class Pipeline {
  public:
    using Stage = std::pair<uint32_t, uint32_t>;

    Pipeline() = default;

    virtual ~Pipeline() = default;

    void BindPipeline(const CommandBuffer &commandBuffer) const {
        vkCmdBindPipeline(commandBuffer, getPipelineBindPoint(), getPipeline());
    }

    virtual const Shader *GetShader() const = 0;
    virtual bool IsPushDescriptors() const = 0;
    virtual const VkDescriptorSetLayout &getDescriptorSetLayout() const = 0;
    virtual const VkDescriptorPool &getDescriptorPool() const = 0;
    virtual const VkPipeline &getPipeline() const = 0;
    virtual const VkPipelineLayout &getPipelineLayout() const = 0;
    virtual const VkPipelineBindPoint getPipelineBindPoint() const = 0;

  private:
  private:
};

} // namespace Worlds
