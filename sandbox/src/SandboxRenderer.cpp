#include "SandboxRenderer.hpp"

#include "Worlds/Graphics/Buffers/UniformHandler.hpp"
#include "Worlds/Graphics/Descriptors/DescriptorsHandler.hpp"
#include "Worlds/Graphics/Pipelines/PipelineGraphics.hpp"

#include "Worlds/Models/Shapes/Cube.hpp"
#include "Worlds/Models/Vertex3d.hpp"
#include "Worlds/Scenes/Scenes.hpp"
#include <glm/glm.hpp>

#include "Worlds/Meshes/MeshSubrender.hpp"

SandboxRenderer::SandboxRenderer() {
    std::vector<Attachment> renderpassAttachments = {
        {0, "Swapchain", Attachment::Type::Swapchain, false,
         VK_FORMAT_UNDEFINED, Color::Black}};

    std::vector<SubpassType> renderpassSubpasses = {{0, {0}}};

    AddRenderStage(std::make_unique<RenderStage>(renderpassAttachments,
                                                 renderpassSubpasses));
}

void SandboxRenderer::Start() { AddSubrender<Worlds::MeshSubrender>({0, 0}); }

void SandboxRenderer::Update() {}
