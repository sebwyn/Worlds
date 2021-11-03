#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Worlds {

class Transform {
  public:
    Transform() = default;

    glm::vec3 GetPosition() { return position; }
    void SetPosition(glm::vec3 &&position) { this->position = position; }

    glm::vec3 GetRotation() { return rotation; }
    void SetRotation(glm::vec3 &&rotation) { this->rotation = rotation; }

    glm::vec3 GetScale() { return scale; }
    void SetScale(glm::vec3 &&scale) { this->scale = scale; }

    void UpdateMatrix() {
        glm::mat4 transMat = glm::translate(glm::mat4(1.0), position);
        glm::mat4 rotMat = glm::mat4_cast(glm::quat(rotation));
        glm::mat4 scaleMat = glm::scale(glm::mat4(1.0), scale);
        matrix = transMat * rotMat * scaleMat;
    }

    glm::mat4 GetMatrix() { return matrix; }

  private:
    glm::mat4 matrix;

    glm::vec3 position = glm::vec3(0);
    glm::vec3 rotation = glm::vec3(0);
    glm::vec3 scale = glm::vec3(1);
};

} // namespace Worlds
