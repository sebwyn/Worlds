#pragma once

#include "Platform/Vulkan/RenderStage.hpp"
#include "Platform/Vulkan/SubrenderHolder.hpp"
#include "Platform/Vulkan/Pipeline/Pipeline.hpp"

namespace Worlds {

class Renderer {
    friend class VKRenderAPI;
  public:
    Renderer() = default;

    virtual ~Renderer() = default;

    virtual void Start() = 0;

    virtual void onUpdate() = 0;

    template <typename T> T *getSubrender() const {
        return m_subrender_holder.get<T>();
    }

    RenderStage *getRenderStage(uint32_t index) {
        if (m_renderstages.empty() || m_renderstages.size() < index)
            return nullptr;

        return m_renderstages.at(index).get();
    }

  protected:
    template <typename T, typename... Args>
    T *addSubrender(const Pipeline::Stage &pipelineStage, Args &&...args) {
        return m_subrender_holder.add<T>(
            pipelineStage,
            Scope<T>(pipelineStage, std::forward<Args>(args)...));
    }

    template <typename T> void removeSubrender() {
        m_subrender_holder.remove<T>();
    }

    void ClearSubrenders() {
        m_subrender_holder.clear();
    }

    void addRenderStage(Scope<RenderStage> &&renderStage){
        m_renderstages.emplace_back(std::move(renderStage));
    }

  protected:
    SubrenderHolder m_subrender_holder;
    std::vector<Scope<RenderStage>> m_renderstages;
};

} // namespace Worlds
