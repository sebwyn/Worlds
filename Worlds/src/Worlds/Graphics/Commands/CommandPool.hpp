#pragma once

#include <thread>
#include <volk.h>

namespace Worlds {
/**
 * @brief Class that represents a command pool.
 */
class CommandPool {
  public:
    explicit CommandPool(
        const std::thread::id &threadId = std::this_thread::get_id());

    ~CommandPool();

    operator const VkCommandPool &() const { return commandPool; }

    const VkCommandPool &GetCommandPool() const { return commandPool; }
    const std::thread::id &GetThreadId() const { return threadId; }

  private:
    VkCommandPool commandPool = VK_NULL_HANDLE;
    std::thread::id threadId;
};
} // namespace Worlds
