#include "Platform/Vulkan/Command/CommandBuffer.hpp"

#include "Platform/Vulkan/VKRenderAPI.hpp"

using namespace Worlds;

CommandBuffer::CommandBuffer(bool begin, VkQueueFlagBits queueType,
                             VkCommandBufferLevel bufferLevel)
    : m_command_pool(VKRenderAPI::get()->getCommandPool()),
      m_queue_type(queueType) {

    auto logicalDevice = VKRenderAPI::get()->getLogicalDevice();

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = *m_command_pool;
    allocInfo.level = bufferLevel;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(logicalDevice->getLogicalDevice(), &allocInfo,
                                 &m_command_buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    if (begin)
        this->begin();
}

CommandBuffer::~CommandBuffer() {
    auto logicalDevice = VKRenderAPI::get()->getLogicalDevice();

    vkFreeCommandBuffers(logicalDevice->getLogicalDevice(),
                         m_command_pool->getCommandPool(), 1,
                         &m_command_buffer);
}

void CommandBuffer::begin(VkCommandBufferUsageFlags usage) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = usage;

    vkBeginCommandBuffer(m_command_buffer, &beginInfo);
    m_running = true;
}
void CommandBuffer::end() {
    if (!m_running)
        return;

    vkEndCommandBuffer(m_command_buffer);
    m_running = false;
}
void CommandBuffer::submitIdle() {
    auto logicalDevice = VKRenderAPI::get()->getLogicalDevice();
    auto queueSelected = getQueue();

    if (m_running)
        end();

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_command_buffer;

    VkFenceCreateInfo fenceCreateInfo = {};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

    VkFence fence;
    vkCreateFence(logicalDevice->getLogicalDevice(), &fenceCreateInfo, nullptr,
                  &fence);
    vkResetFences(logicalDevice->getLogicalDevice(), 1, &fence);
    vkQueueSubmit(queueSelected, 1, &submitInfo, fence);
    vkWaitForFences(logicalDevice->getLogicalDevice(), 1, &fence, VK_TRUE,
                    std::numeric_limits<uint64_t>::max());
    vkDestroyFence(logicalDevice->getLogicalDevice(), fence, nullptr);
}

VkQueue CommandBuffer::getQueue() const {
    auto logicalDevice = VKRenderAPI::get()->getLogicalDevice();

    switch (m_queue_type) {
    case VK_QUEUE_GRAPHICS_BIT:
        return logicalDevice->getGraphicsQueue();
    case VK_QUEUE_COMPUTE_BIT:
        return logicalDevice->getComputeQueue();
    default:
        return nullptr;
    }
}
