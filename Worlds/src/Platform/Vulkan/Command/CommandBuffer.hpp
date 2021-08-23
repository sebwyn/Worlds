#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Platform/Vulkan/Command/CommandPool.hpp"

namespace Worlds {

class CommandBuffer {
  public:
    CommandBuffer(
        bool begin = true, VkQueueFlagBits queueType = VK_QUEUE_GRAPHICS_BIT,
        VkCommandBufferLevel bufferLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    ~CommandBuffer();

    void begin(VkCommandBufferUsageFlags usage =
                   VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    void end();

    void submitIdle();

    operator const VkCommandBuffer &() const { return m_command_buffer; }

  private:
    VkQueue getQueue() const;

  private:
    Ref<CommandPool> m_command_pool;

    VkQueueFlagBits m_queue_type;
    VkCommandBuffer m_command_buffer = VK_NULL_HANDLE;
    bool m_running = false;
};

} // namespace Worlds
