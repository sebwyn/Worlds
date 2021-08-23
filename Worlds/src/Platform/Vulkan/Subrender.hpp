#pragma once

#include "Platform/Vulkan/Command/CommandBuffer.hpp"
#include "Platform/Vulkan/Pipeline/Pipeline.hpp"
#include "Worlds/Utils/TypeInfo.hpp"

namespace Worlds {

class Subrender {
  public:
    Subrender(Pipeline::Stage stage) : m_stage(std::move(stage)) {}

    virtual ~Subrender() = default;

    virtual void render(const CommandBuffer &commandBuffer) = 0;

    const Pipeline::Stage &getStage() const { return m_stage; }

    bool isEnabled() const { return m_enabled; }
    void setEnabled(bool enable) { m_enabled = enable; }

  private:
    bool m_enabled = true;
    Pipeline::Stage m_stage;
};

template class TypeInfo<Subrender>;
} // namespace Worlds
