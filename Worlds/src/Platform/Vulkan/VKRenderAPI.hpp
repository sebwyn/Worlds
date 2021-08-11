#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <optional>
#include <set>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "Worlds/Renderer/RenderAPI.hpp"

#include "Platform/Vulkan/Devices/Instance.hpp"
#include "Platform/Vulkan/Devices/Surface.hpp"
#include "Platform/Vulkan/Devices/PhysicalDevice.hpp"
#include "Platform/Vulkan/Devices/LogicalDevice.hpp"

namespace Worlds {

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3>
    getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3>
            attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }

    bool operator==(const Vertex &other) const {
        return pos == other.pos && color == other.color &&
               texCoord == other.texCoord;
    }
};

struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};
} // namespace Worlds

namespace std {
template <> struct hash<Worlds::Vertex> {
    size_t operator()(Worlds::Vertex const &vertex) const {
        return ((hash<glm::vec3>()(vertex.pos) ^
                 (hash<glm::vec3>()(vertex.color) << 1)) >>
                1) ^
               (hash<glm::vec2>()(vertex.texCoord) << 1);
    }
};
} // namespace std

namespace Worlds {

class VKRenderAPI : public RenderAPI {
  public:
    VKRenderAPI(GLFWwindow* window);
    virtual ~VKRenderAPI();

    void onWindowResize(WindowResizeEvent& e);
    void onUpdate();

    //Renderer* getRenderer(){ return m_renderer; }

  private:
    //void recreateSwapChain(); 
    //void startRenderPass(); 
    //void endRenderPass();

  private:
    Scope<Instance> m_instance;
    Scope<Surface> m_surface;
    Scope<PhysicalDevice> m_physical_device;
    Scope<LogicalDevice> m_logical_device;
    
    std::size_t m_current_frame = 0;
    bool m_framebuffer_resized = false;

    std::vector<VkSemaphore> m_present_completes;
    std::vector<VkSemaphore> m_render_completes;
    std::vector<VkFence> m_in_flight_fences;
    /*
    Scope<Swapchain> m_swapchain;

    std::vector<Scope<CommandBuffer>> m_command_buffers;

    Scope<Renderer> m_renderer;
    */

  private:
    void init();
    void destroy();

    void mainLoop();
    void cleanupSwapChain();
    void recreateSwapChain();
    void createSwapChain();
    void createImageViews();
    void createRenderPass();
    void createDescriptorSetLayout();
    void createGraphicsPipeline();
    void createFramebuffers();
    void createCommandPool();
    void createColorResources();
    void createDepthResources();
    VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates,
                                 VkImageTiling tiling,
                                 VkFormatFeatureFlags features);
    VkFormat findDepthFormat();
    bool hasStencilComponent(VkFormat format);
    void createTextureImage();
    void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth,
                         int32_t texHeight, uint32_t mipLevels);
    void createTextureImageView();
    void createTextureSampler();
    VkImageView createImageView(VkImage image, VkFormat format,
                                VkImageAspectFlags aspectFlags,
                                uint32_t mipLevels);
    void createImage(uint32_t width, uint32_t height, uint32_t mipLevels,
                     VkSampleCountFlagBits numSamples, VkFormat format,
                     VkImageTiling tiling, VkImageUsageFlags usage,
                     VkMemoryPropertyFlags properties, VkImage &image,
                     VkDeviceMemory &imageMemory);
    void transitionImageLayout(VkImage image, VkFormat format,
                               VkImageLayout oldLayout, VkImageLayout newLayout,
                               uint32_t mipLevels);
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width,
                           uint32_t height);
    void loadModel();
    void createVertexBuffer();
    void createIndexBuffer();
    void createUniformBuffers();
    void createDescriptorPool();
    void createDescriptorSets();
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                      VkMemoryPropertyFlags properties, VkBuffer &buffer,
                      VkDeviceMemory &bufferMemory);
    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    uint32_t findMemoryType(uint32_t typeFilter,
                            VkMemoryPropertyFlags properties);
    void createCommandBuffers();
    void createSyncObjects();
    void updateUniformBuffer(uint32_t currentImage);
    void drawFrame();
    VkShaderModule createShaderModule(const std::vector<char> &code);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(
        const std::vector<VkPresentModeKHR> &availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
    static std::vector<char> readFile(const std::string &filename);

  private:
    GLFWwindow *m_window;

    VkSwapchainKHR m_swapChain;
    ::std::vector<VkImage> m_swapChainImages;
    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;
    ::std::vector<VkImageView> m_swapChainImageViews;
    ::std::vector<VkFramebuffer> m_swapChainFramebuffers;

    VkRenderPass m_renderPass;
    VkDescriptorSetLayout m_descriptorSetLayout;
    VkPipelineLayout m_pipelineLayout;
    VkPipeline m_graphicsPipeline;

    VkCommandPool m_commandPool;

    VkImage m_colorImage;
    VkDeviceMemory m_colorImageMemory;
    VkImageView m_colorImageView;

    VkImage m_depthImage;
    VkDeviceMemory m_depthImageMemory;
    VkImageView m_depthImageView;

    uint32_t m_mipLevels;
    VkImage m_textureImage;
    VkDeviceMemory m_textureImageMemory;
    VkImageView m_textureImageView;
    VkSampler m_textureSampler;

    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
    VkBuffer m_vertexBuffer;
    VkDeviceMemory m_vertexBufferMemory;
    VkBuffer m_indexBuffer;
    VkDeviceMemory m_indexBufferMemory;

    std::vector<VkBuffer> m_uniformBuffers;
    std::vector<VkDeviceMemory> m_uniformBuffersMemory;

    VkDescriptorPool m_descriptorPool;
    std::vector<VkDescriptorSet> m_descriptorSets;

    std::vector<VkCommandBuffer> m_commandBuffers;
    std::vector<VkFence> m_imagesInFlight;
};

} // namespace Worlds
