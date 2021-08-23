#include "Platform/Vulkan/SubrenderHolder.hpp"

using namespace Worlds;

void SubrenderHolder::clear() { m_stages.clear(); }

void SubrenderHolder::removeSubrenderStage(const TypeId &id) {
    for (auto it = m_stages.begin(); it != m_stages.end();) {
        if (it->second == id) {
            it = m_stages.erase(it);
        } else {
            ++it;
        }
    }
}

void SubrenderHolder::renderStage(const Pipeline::Stage &stage,
                                  const CommandBuffer &commandBuffer) {
    for (const auto &[stageIndex, typeId] : m_stages) {
        if (stageIndex.first != stage) {
            continue;
        }

        if (auto &subrender = m_subrenders[typeId]) {
            if (subrender->isEnabled()) {
                subrender->render(commandBuffer);
            }
        }
    }
}
