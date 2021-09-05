#pragma once

#include "Worlds/Core/Base.hpp"

#include "Worlds/Graphics/Buffers/Buffer.hpp"

namespace Worlds {

/* add a factory for models, so once a model is loaded it will never be loaded
 * again */
/* the same system will be implemented for materials */
/*  */
/* in reality the factory should only hold on to the model for as it is needed,
 * say within a world */
/* or a portion of a world */

class Model {
  public:
    Model() = default;
    template <typename T>
    explicit Model(const std::vector<T> &vertices,
                   const std::vector<uint32_t> &indices);

    bool CmdRender(const CommandBuffer &commandBuffer, uint32_t instances);

    template <typename T> std::vector<T> GetVertices(std::size_t offset) const;
    template <typename T> void SetVertices(const std::vector<T> &vertices);

    std::vector<uint32_t> GetIndices(std::size_t offset) const;
    void SetIndices(const std::vector<uint32_t> &indices);

    Buffer *GetVertexBuffer() { return vertexBuffer.get(); }
    Buffer *GetIndexBuffer() { return vertexBuffer.get(); }
    uint32_t GetVertexCount() const { return vertexCount; }
    uint32_t GetIndexCount() const { return indexCount; }
    static VkIndexType GetIndexType() { return VK_INDEX_TYPE_UINT32; }

  protected:
    template <typename T>
    void Initialize(const std::vector<T> &vertices,
                    const std::vector<uint32_t> &indices);

  protected:
    Scope<Buffer> vertexBuffer;
    Scope<Buffer> indexBuffer;
    uint32_t vertexCount = 0;
    uint32_t indexCount = 0;

    glm::vec3 minExtents;
    glm::vec3 maxExtents;

    float radius;
};

template <typename T>
Model::Model(const std::vector<T> &vertices,
             const std::vector<uint32_t> &indices)
    : Model() {
    Initialize(vertices, indices);
}

template <typename T>
std::vector<T> Model::GetVertices(std::size_t offset) const {
    Buffer vertexStaging(vertexBuffer->GetSize(),
                         VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                             VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                             VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    CommandBuffer commandBuffer;

    VkBufferCopy copyRegion = {};
    copyRegion.size = vertexStaging.GetSize();
    vkCmdCopyBuffer(commandBuffer, vertexBuffer->GetBuffer(),
                    vertexStaging.GetBuffer(), 1, &copyRegion);

    commandBuffer.SubmitIdle();

    void *verticesMemory;
    vertexStaging.MapMemory(&verticesMemory);
    std::vector<T> vertices(vertexCount);

    auto sizeOfSrcT = vertexStaging.GetSize() / vertexCount;

    for (uint32_t i = 0; i < vertexCount; i++) {
        std::memcpy(&vertices[i],
                    static_cast<char *>(verticesMemory) + (i * sizeOfSrcT) +
                        offset,
                    sizeof(T));
    }

    vertexStaging.UnmapMemory();
    return vertices;
}

template <typename T> void Model::SetVertices(const std::vector<T> &vertices) {
    vertexBuffer = nullptr;
    vertexCount = static_cast<uint32_t>(vertices.size());

    if (vertices.empty())
        return;

    Buffer vertexStaging(sizeof(T) * vertices.size(),
                         VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                             VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         vertices.data());
    vertexBuffer = std::make_unique<Buffer>(
        vertexStaging.GetSize(),
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
            VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    CommandBuffer commandBuffer;

    VkBufferCopy copyRegion = {};
    copyRegion.size = vertexStaging.GetSize();
    vkCmdCopyBuffer(commandBuffer, vertexStaging.GetBuffer(),
                    vertexBuffer->GetBuffer(), 1, &copyRegion);

    commandBuffer.SubmitIdle();
}

template <typename T>
void Model::Initialize(const std::vector<T> &vertices,
                       const std::vector<uint32_t> &indices) {
    SetVertices(vertices);
    SetIndices(indices);

    minExtents = glm::vec3(-std::numeric_limits<float>::infinity());
    maxExtents = glm::vec3(std::numeric_limits<float>::infinity());

    for (const auto &vertex : vertices) {
        glm::vec3 position(vertex.position);
        minExtents = glm::min(position, minExtents);
        maxExtents = glm::max(position, maxExtents);
    }

    radius = std::max(glm::length(minExtents), glm::length(maxExtents));
}

} // namespace Worlds
