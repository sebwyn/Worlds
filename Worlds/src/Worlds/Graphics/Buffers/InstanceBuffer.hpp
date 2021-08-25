#pragma once

#include "Worlds/Graphics/Buffers/Buffer.hpp"

namespace Worlds {
class InstanceBuffer : public Buffer {
  public:
    explicit InstanceBuffer(VkDeviceSize size);

    void Update(const CommandBuffer &commandBuffer, const void *newData);
};
} // namespace Worlds
