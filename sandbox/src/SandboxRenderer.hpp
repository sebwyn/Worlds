#pragma once

#include "Worlds/Graphics/Renderer.hpp"

using namespace Worlds;

class SandboxRenderer : public Renderer {
  public:
    SandboxRenderer();

    void SetColor(glm::vec4 color);

    void Start() override;
    void Update() override;

  private:
};
