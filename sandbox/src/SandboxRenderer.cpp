#include "SandboxRenderer.hpp"

#include "Worlds/Graphics/Buffers/UniformHandler.hpp"
#include "Worlds/Graphics/Descriptors/DescriptorsHandler.hpp"
#include "Worlds/Graphics/Pipelines/PipelineGraphics.hpp"

#include "Worlds/Models/Shapes/Cube.hpp"
#include "Worlds/Models/Vertex3d.hpp"
#include "Worlds/Scenes/Scenes.hpp"
#include <glm/glm.hpp>

class SandboxSubrender : public Subrender {
  public:
    SandboxSubrender(const Pipeline::Stage &stage)
        : Subrender(stage),
          coloredPipeline(stage,
                          {"../Sandbox/assets/shaders/scene.frag",
                           "../Sandbox/assets/shaders/scene.vert"},
                          {Vertex3d::GetVertexInput()}, {},
                          PipelineGraphics::Mode::Polygon,
                          PipelineGraphics::Depth::None) {}

    void SetColor(glm::vec4 color) { this->color = color; }

    void Render(const CommandBuffer &commandBuffer) override {
        auto camera = Scenes::Get()->GetCamera();

        coloredPipeline.BindPipeline(commandBuffer);

        uniformScene.Push("projection", camera->GetProjectionMatrix());
        uniformScene.Push("view", camera->GetViewMatrix());
        descriptors.Push("UniformScene", uniformScene);

        uniformObject.Push("transform", glm::mat4(1.0));
        uniformObject.Push("color", color);
        descriptors.Push("UniformObject", uniformObject);

        if (!descriptors.Update(coloredPipeline))
            return;

        descriptors.BindDescriptor(commandBuffer, coloredPipeline);

        cube.CmdRender(commandBuffer, 1);
    }

  private:
    glm::vec4 color = glm::vec4(1, 0, 1, 1);
    Cube cube = Cube(glm::vec3(1.0));

    // Order is important: the pipeline will be freed after the descriptors
    PipelineGraphics coloredPipeline;

    DescriptorsHandler descriptors;
    UniformHandler uniformScene;
    UniformHandler uniformObject;
};

SandboxRenderer::SandboxRenderer() {
    std::vector<Attachment> renderpassAttachments = {
        {0, "Swapchain", Attachment::Type::Swapchain, false,
         VK_FORMAT_UNDEFINED, Color::Black}};

    std::vector<SubpassType> renderpassSubpasses = {{0, {0}}};

    AddRenderStage(std::make_unique<RenderStage>(renderpassAttachments,
                                                 renderpassSubpasses));
}

void SandboxRenderer::Start() { AddSubrender<SandboxSubrender>({0, 0}); }

void SandboxRenderer::SetColor(glm::vec4 color) {
    GetSubrender<SandboxSubrender>()->SetColor(color);
}

void SandboxRenderer::Update() {}
