#pragma once

#include "Platform/Vulkan/Subrender.hpp"

namespace Worlds {

class SubrenderHolder {
    friend class VKRenderAPI;

  public:
    template <typename T> T *get() const {
        const auto typeId = TypeInfo<Subrender>::getTypeId<T>();

        if (auto it = m_subrenders.find(typeId);
            it != m_subrenders.end() && it->second)
            return static_cast<T *>(it->second.get());

        return nullptr;
    }
    template <typename T>
    T *add(const Pipeline::Stage &stage, Scope<T> &&subrender) {
        const auto typeId = TypeInfo<Subrender>::getTypeId<T>();

        m_stages.insert({StageIndex(stage, m_subrenders.size()), typeId});

        m_subrenders[typeId] = std::move(subrender);
    }
    template <typename T> void remove() {
        const auto typeId = TypeInfo<Subrender>::getTypeId<T>();

        removeSubrenderStage(typeId);

        m_subrenders.erase(typeId);
    }

    void clear();

  private:
    using StageIndex = std::pair<Pipeline::Stage, std::size_t>;

    void removeSubrenderStage(const TypeId &id);

    void renderStage(const Pipeline::Stage &stage,
                     const CommandBuffer &commandBuffer);

    std::unordered_map<TypeId, Scope<Subrender>> m_subrenders;
    std::multimap<StageIndex, TypeId> m_stages;
};

} // namespace Worlds
