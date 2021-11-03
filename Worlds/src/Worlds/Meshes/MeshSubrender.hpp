#pragma once

#include "Worlds/Graphics/Buffers/UniformHandler.hpp"
#include "Worlds/Graphics/Descriptors/DescriptorsHandler.hpp"
#include "Worlds/Graphics/Pipelines/PipelineGraphics.hpp"
#include "Worlds/Graphics/Subrender.hpp"

namespace Worlds {

// subrender in the first part of the pipeline for
class MeshSubrender : public Subrender {
  public:
    MeshSubrender(Pipeline::Stage stage);

    void Render(const CommandBuffer &commandBuffer) override;

  private:
    // Order is important: the pipeline will be freed after the descriptors
    PipelineGraphics coloredPipeline;

    UniformHandler uniformScene;
};

} // namespace Worlds
