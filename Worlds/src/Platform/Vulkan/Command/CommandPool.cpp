#include "Platform/Vulkan/Command/CommandPool.hpp"

#include "Platform/Vulkan/VKRenderAPI.hpp"

using namespace Worlds;

CommandPool::CommandPool(){
    auto logicalDevice = VKRenderAPI::get()->getLogicalDevice();

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = logicalDevice->getGraphicsFamily();

    if (vkCreateCommandPool(logicalDevice->getLogicalDevice(), &poolInfo,
                            nullptr, &m_command_pool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics command pool!");
    }
}

CommandPool::~CommandPool(){
    auto logicalDevice = VKRenderAPI::get()->getLogicalDevice();

    vkDestroyCommandPool(logicalDevice->getLogicalDevice(), m_command_pool, nullptr);
}
