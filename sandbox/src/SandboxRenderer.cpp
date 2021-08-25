#include "SandboxRenderer.hpp"

#include "Worlds/Graphics/Pipelines/PipelineGraphics.hpp"

class SandboxSubrender : public Subrender {
  public:
    SandboxSubrender(const Pipeline::Stage &stage)
        : Subrender(stage), pipeline(stage,
                                     {"../Sandbox/assets/shaders/test.frag",
                                      "../Sandbox/assets/shaders/test.vert"},
                                     {}, {}, PipelineGraphics::Mode::Polygon,
                                     PipelineGraphics::Depth::None) {}

    void Render(const CommandBuffer &commandBuffer) override {
        pipeline.BindPipeline(commandBuffer);
        vkCmdDraw(commandBuffer, 3, 1, 0, 0);
    }

  private:
    PipelineGraphics pipeline;
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

void SandboxRenderer::Update() {}
