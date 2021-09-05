#pragma once

#include "Worlds/Graphics/Pipelines/Shader.hpp"

namespace Worlds {

class Vertex3d {
  public:
    Vertex3d() = default;

    Vertex3d(glm::vec3 position, glm::vec2 uv, glm::vec3 normal)
        : position(position), uv(uv), normal(normal) {}

    static Shader::VertexInput GetVertexInput(uint32_t baseBinding = 0) {
        std::vector<VkVertexInputBindingDescription> bindingDescriptions = {
            {baseBinding, sizeof(Vertex3d), VK_VERTEX_INPUT_RATE_VERTEX}};
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {
            {0, baseBinding, VK_FORMAT_R32G32B32_SFLOAT,
             offsetof(Vertex3d, position)},
            {1, baseBinding, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex3d, uv)},
            {2, baseBinding, VK_FORMAT_R32G32B32_SFLOAT,
             offsetof(Vertex3d, normal)}};
        return {bindingDescriptions, attributeDescriptions};
    }

    bool operator==(const Vertex3d &rhs) {
        return position == rhs.position && uv == rhs.uv && normal == rhs.normal;
    }

    bool operator!=(const Vertex3d &rhs) { return !operator==(rhs); }

    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;
};

} // namespace Worlds
