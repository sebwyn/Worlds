#pragma once

#include "Worlds/Scenes/Camera.hpp"

#include "Worlds/Events/Event.hpp"
#include "Worlds/Events/KeyboardEvents.hpp"
#include "Worlds/Events/MouseEvents.hpp"

namespace Worlds {

class SandboxCamera : public Worlds::Camera {
  public:
    SandboxCamera();

    void UpdateCam() override;

    bool OnEvent(Worlds::Event &e) override;
    bool OnKeyPressed(KeyPressedEvent &e);
    bool OnKeyReleased(KeyReleasedEvent &e);

    bool OnMouseMove(MouseMovedEvent &e);

    void calcTranslation(float dt);
    void calcRotation(float dt);

  private:
    float speed = 10;
    float mouseSpeed = 0.5;

    glm::vec3 move = glm::vec3(0);
    glm::vec2 rotate = glm::vec2(0);

    glm::vec3 translationVector = glm::vec3(0);
    glm::vec2 rotationVector = glm::vec2(0);
};

} // namespace Worlds
