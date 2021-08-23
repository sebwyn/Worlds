#include "Platform/Vulkan/Buffer/Buffer.hpp"

#include "Platform/Vulkan/VKRenderAPI.hpp"

using namespace Worlds;

Buffer::Buffer(VkDeviceSize size, VkBufferUsageFlags usage,
               VkMemoryPropertyFlags properties, const void *data) {
    auto logicalDevice = VKRenderAPI::get()->getLogicalDevice();

    auto graphicsFamily = logicalDevice->getGraphicsFamily();
    auto presentFamily = logicalDevice->getPresentFamily();
    auto computeFamily = logicalDevice->getComputeFamily();

    std::array queueFamily = {graphicsFamily, presentFamily, computeFamily};

    VkBufferCreateInfo bufferCreateInfo{};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.size = size;
    bufferCreateInfo.usage = usage;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferCreateInfo.queueFamilyIndexCount =
        static_cast<uint32_t>(queueFamily.size());
    bufferCreateInfo.pQueueFamilyIndices = queueFamily.data();
    vkCreateBuffer(logicalDevice->getLogicalDevice(), &bufferCreateInfo,
                   nullptr, &m_buffer);

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(logicalDevice->getLogicalDevice(), m_buffer,
                                  &memoryRequirements);

    VkMemoryAllocateInfo memoryAllocateInfo = {};
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.allocationSize = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex =
        findMemoryType(memoryRequirements.memoryTypeBits, properties);
    vkAllocateMemory(logicalDevice->getLogicalDevice(), &memoryAllocateInfo,
                     nullptr, &m_memory);

    if (data) {

        void *mapped;
        mapMemory(&mapped);
        std::memcpy(mapped, data, size);

        if ((properties & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == 0) {
            VkMappedMemoryRange mappedMemoryRange{};
            mappedMemoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
            mappedMemoryRange.memory = m_memory;
            mappedMemoryRange.offset = 0;
            mappedMemoryRange.size = size;
            vkFlushMappedMemoryRanges(logicalDevice->getLogicalDevice(), 1,
                                      &mappedMemoryRange);
        }
        unmapMemory();
    }
}

Buffer::~Buffer() {
    auto logicalDevice = VKRenderAPI::get()->getLogicalDevice();

    vkDestroyBuffer(logicalDevice->getLogicalDevice(), m_buffer, nullptr);
    vkFreeMemory(logicalDevice->getLogicalDevice(), m_memory, nullptr);
}

void Buffer::mapMemory(void **data) {
    auto logicalDevice = VKRenderAPI::get()->getLogicalDevice();
    vkMapMemory(logicalDevice->getLogicalDevice(), m_memory, 0, m_size, 0,
                data);
}

void Buffer::unmapMemory() {
    auto logicalDevice = VKRenderAPI::get()->getLogicalDevice();
    vkUnmapMemory(logicalDevice->getLogicalDevice(), m_memory);
}

uint32_t
Buffer::findMemoryType(uint32_t typeFilter,
                       const VkMemoryPropertyFlags &requiredProperties) {
    auto physicalDevice = VKRenderAPI::get()->getPhysicalDevice();
    auto memoryProperties = physicalDevice->getMemoryProperties();

    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
        uint32_t memoryTypeBits = 1 << i;

        if (typeFilter & memoryTypeBits &&
            (memoryProperties.memoryTypes[i].propertyFlags &
             requiredProperties) == requiredProperties) {
            return i;
        }

        std::runtime_error("Failed to find a valid memory type for buffer");
    }
}
