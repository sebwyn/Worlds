#pragma once

#include "Worlds/Scenes/Camera.hpp"

#include "Worlds/Events/Event.hpp"

namespace Worlds {

class SandboxCamera : public Worlds::Camera {
  public:
    void UpdateCam() override;

    bool OnEvent(Worlds::Event &e) override;

  private:
};

} // namespace Worlds
