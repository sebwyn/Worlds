#pragma once

#include "Worlds/Graphics/Renderer.hpp"

using namespace Worlds;

class SandboxRenderer : public Renderer {
  public:
    SandboxRenderer();

    void Start() override;
    void Update() override;

  private:
};
