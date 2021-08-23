#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Worlds {

class Buffer {
  public:
    Buffer(VkDeviceSize size, VkBufferUsageFlags usage,
           VkMemoryPropertyFlags properties, const void *data = nullptr);

    ~Buffer();

    void mapMemory(void **data);
    void unmapMemory();

    VkBuffer getBuffer() const { return m_buffer; }
    VkDeviceSize getSize() const { return m_size; }

    static uint32_t
    findMemoryType(uint32_t typeFilter,
                   const VkMemoryPropertyFlags &requiredProperties);

  private:
  private:
    VkDeviceSize m_size;
    VkBuffer m_buffer = VK_NULL_HANDLE;
    VkDeviceMemory m_memory = VK_NULL_HANDLE;
};

} // namespace Worlds
