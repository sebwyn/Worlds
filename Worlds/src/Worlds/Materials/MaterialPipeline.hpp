#pragma once

#include "Worlds/Graphics/Pipelines/PipelineGraphics.hpp"

namespace Worlds {

class MaterialPipeline {
  public:
  private:
    PipelineGraphicsCreate pipelineCreate;

    Scope<PipelineGraphics> pipeline;
};

} // namespace Worlds
