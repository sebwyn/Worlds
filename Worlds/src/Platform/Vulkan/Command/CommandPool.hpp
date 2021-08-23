#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Worlds {

class CommandPool {
public:
    CommandPool();

    ~CommandPool();

    operator const VkCommandPool &() const { return m_command_pool; }

    const VkCommandPool &getCommandPool(){ return m_command_pool; } 
private:
private:
    VkCommandPool m_command_pool = VK_NULL_HANDLE;
};

};
