#pragma once

#include "Worlds/Core/Window.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Worlds {

class Camera {
  public:
    Camera() : near(0.1), far(1000), fov(45) {}

    virtual void UpdateCam() {}
    virtual bool OnEvent(Event &e) { return false; }

    void Update() {
        UpdateCam();

        UpdateProjectionMatrix();
        UpdateViewMatrix();
    }

    const glm::mat4 &GetProjectionMatrix() { return projection; }
    const glm::mat4 &GetViewMatrix() { return view; }

    const float &GetNear() { return near; }
    void SetNear(float near) { this->near = near; }

    const float &GetFar() { return far; }
    void SetFar(float far) { this->far = far; }

    const float &GetFov() { return fov; }
    void SetFov(float fov) { this->fov = fov; }

    const glm::vec3 &GetPosition() { return position; }
    void SetPosition(const glm::vec3 &position) { this->position = position; }

    const glm::vec3 &GetRotation() { return rotation; }
    void SetRotation(const glm::vec3 &rotation) { this->rotation = rotation; }

  protected:
    void UpdateProjectionMatrix() {
        glm::uvec2 window = WindowAPI::Get()->GetSize();
        projection =
            glm::perspective(fov, (float)window.x / window.y, near, far);
    }
    void UpdateViewMatrix() {
        // get look vector from rotation
        glm::mat4 rotationMatrix = glm::mat4_cast(glm::quat(rotation));
        glm::vec3 look = glm::vec3(rotationMatrix * glm::vec4(0, 0, -1, 0));
        // use look vector to calculate view
        view = glm::lookAt(position, position + look, glm::vec3(0, 1, 0));
    }

  protected:
    float near, far;
    float fov;

    glm::vec3 position;
    glm::vec3 rotation;

    glm::mat4 view, projection;
};

} // namespace Worlds
