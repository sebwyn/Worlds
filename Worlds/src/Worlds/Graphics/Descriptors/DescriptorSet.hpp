#pragma once

#include "Worlds/Graphics/Commands/CommandBuffer.hpp"
#include "Worlds/Graphics/Pipelines/Pipeline.hpp"

namespace Worlds {
class Descriptor;
class WriteDescriptorSet;

class DescriptorSet {
  public:
    explicit DescriptorSet(const Pipeline &pipeline);
    ~DescriptorSet();

    static void
    Update(const std::vector<VkWriteDescriptorSet> &descriptorWrites);

    void BindDescriptor(const CommandBuffer &commandBuffer) const;

    const VkDescriptorSet &GetDescriptorSet() const { return descriptorSet; }

  private:
    VkPipelineLayout pipelineLayout;
    VkPipelineBindPoint pipelineBindPoint;
    VkDescriptorPool descriptorPool;
    VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
};
} // namespace Worlds
