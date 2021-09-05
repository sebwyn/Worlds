#pragma once

#include <filesystem>
#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>

#include "Worlds/Graphics/RenderStage.hpp"
#include "Worlds/Graphics/SubrenderHolder.hpp"

namespace Worlds {
/**
 * @brief Class used to manage {@link Subrender} objects to create a list of
 * render pass.
 */
class Renderer {
    friend class Graphics;

  public:
    /**
     * Creates a new renderer, fill {@link renderStages} in your subclass of
     * this.
     */
    Renderer() = default;

    virtual ~Renderer() = default;

    /**
     * Run when switching to this scene from another, use this method to create
     * {@link Subrender}'s.
     */
    virtual void Start() = 0;

    /**
     * Run when updating the renderer manager.
     */
    virtual void Update() = 0;

    /**
     * Checks whether a Subrender exists or not.
     * @tparam T The Subrender type.
     * @return If the Subrender has the System.
     */
    /*template<typename T>
    bool HasSubrender() const  {
            return subrenderHolder.Has<T>();
    }*/

    /**
     * Gets a Subrender.
     * @tparam T The Subrender type.
     * @return The Subrender.
     */
    template <typename T> T *GetSubrender() const {
        return subrenderHolder.Get<T>();
    }

    RenderStage *GetRenderStage(uint32_t index) const {
        if (renderStages.empty() || renderStages.size() < index)
            return nullptr;

        return renderStages.at(index).get();
    }

  protected:
    /**
     * Adds a Subrender.
     * @tparam T The Subrender type.
     * @param pipelineStage The Subrender pipeline stage.
     * @tparam Args The constructor arg types.
     * @param args The constructor arguments.
     */
    template <typename T, typename... Args>
    T *AddSubrender(const Pipeline::Stage &pipelineStage, Args &&...args) {
        return subrenderHolder.Add<T>(
            pipelineStage,
            std::make_unique<T>(pipelineStage, std::forward<Args>(args)...));
    }

    /**
     * Removes a Subrender.
     * @tparam T The Subrender type.
     */
    template <typename T> void RemoveSubrender() {
        subrenderHolder.Remove<T>();
    }

    /**
     * Clears all Subrenders.
     */
    void ClearSubrenders() { subrenderHolder.Clear(); }

    void AddRenderStage(std::unique_ptr<RenderStage> &&renderStage) {
        renderStages.emplace_back(std::move(renderStage));
    }

  private:
    bool started = false;
    SubrenderHolder subrenderHolder;
    std::vector<std::unique_ptr<RenderStage>> renderStages;
};
} // namespace Worlds
