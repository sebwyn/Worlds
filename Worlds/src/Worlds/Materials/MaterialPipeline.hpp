#pragma once

#include "Worlds/Graphics/Pipelines/PipelineGraphics.hpp"

namespace Worlds {

class MaterialPipeline {
  public:
    Ref<MaterialPipeline> Create(const Pipeline::Stage &pipelineStage,
                                 const PipelineGraphicsCreate &pipelineCreate);

  private:
    MaterialPipeline(const Pipeline::Stage &pipelineStage,
                     const PipelineGraphicsCreate &pipelineCreate)

        Pipeline::Stage pipelineStage;

    PipelineGraphicsCreate pipelineCreate;
    Scope<PipelineGraphics> pipeline;
};

} // namespace Worlds
