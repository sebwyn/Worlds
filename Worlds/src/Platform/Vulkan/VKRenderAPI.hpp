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
#include "Platform/Vulkan/Devices/LogicalDevice.hpp"
#include "Platform/Vulkan/Devices/PhysicalDevice.hpp"
#include "Platform/Vulkan/Devices/Surface.hpp"

#include "Platform/Vulkan/Renderpass/Swapchain.hpp"

#include "Platform/Vulkan/Command/CommandBuffer.hpp"
#include "Platform/Vulkan/Command/CommandPool.hpp"

#include "Platform/Vulkan/Renderer.hpp"

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
    static void init(GLFWwindow *window) {
        s_instance = CreateScope<VKRenderAPI>(window);
    }
    static VKRenderAPI *get() { return s_instance.get(); }

    VKRenderAPI(GLFWwindow *window);
    virtual ~VKRenderAPI();

    void onWindowResize(WindowResizeEvent &e);
    void onUpdate();

    const PhysicalDevice *getPhysicalDevice() {
        return m_physical_device.get();
    }
    const LogicalDevice *getLogicalDevice() { return m_logical_device.get(); }
    Ref<CommandPool> getCommandPool() { return m_command_pool; }

  private:
    void recreateSwapchain();
    bool startRenderpass(RenderStage &renderpass);
    void endRenderpass(RenderStage &renderpass);

  private:
    static Scope<VKRenderAPI> s_instance;
    GLFWwindow *m_window;

    Scope<Instance> m_instance;
    Scope<Surface> m_surface;
    Scope<PhysicalDevice> m_physical_device;
    Scope<LogicalDevice> m_logical_device;

    Scope<Swapchain> m_swapchain;

    std::size_t m_current_frame = 0;
    bool m_framebuffer_resized = false;

    std::vector<VkSemaphore> m_present_completes;
    std::vector<VkSemaphore> m_render_completes;
    std::vector<VkFence> m_in_flight_fences;

    Ref<CommandPool> m_command_pool;
    std::vector<Scope<CommandBuffer>> m_command_buffers;

    Scope<Renderer> m_renderer;
};

} // namespace Worlds
