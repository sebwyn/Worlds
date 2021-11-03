#include "SandboxCamera.hpp"

#include "Worlds/Core/Window.hpp"

#include <iostream>

using namespace Worlds;
using namespace glm;

SandboxCamera::SandboxCamera() {}

bool SandboxCamera::OnKeyPressed(KeyPressedEvent &e) {
    if (e.getIsRepeat())
        return false;
    switch (e.getKey()) {
    case GLFW_KEY_W:
        move += vec3(0, 0, 1);
        break;
    case GLFW_KEY_A:
        move -= vec3(1, 0, 0);
        break;
    case GLFW_KEY_S:
        move -= vec3(0, 0, 1);
        break;
    case GLFW_KEY_D:
        move += vec3(1, 0, 0);
        break;
    case GLFW_KEY_SPACE:
        move += vec3(0, 1, 0);
        break;
    case GLFW_KEY_LEFT_SHIFT:
        move -= vec3(0, 1, 0);
        break;
    default:
        return false;
    }
    return false;
}

bool SandboxCamera::OnKeyReleased(KeyReleasedEvent &e) {
    switch (e.getKey()) {
    case GLFW_KEY_W:
        move -= vec3(0, 0, 1);
        break;
    case GLFW_KEY_A:
        move += vec3(1, 0, 0);
        break;
    case GLFW_KEY_S:
        move += vec3(0, 0, 1);
        break;
    case GLFW_KEY_D:
        move -= vec3(1, 0, 0);
        break;
    case GLFW_KEY_SPACE:
        move -= vec3(0, 1, 0);
        break;
    case GLFW_KEY_LEFT_SHIFT:
        move += vec3(0, 1, 0);
        break;
    default:
        return false;
    }
    return false;
}

bool SandboxCamera::OnMouseMove(MouseMovedEvent &e) {
    vec2 size = WindowAPI::Get()->GetSize();

    rotate = vec2(0, -float(size.x / 2 - e.getX()));
    rotate += vec2( -float(size.y / 2 - e.getY()), 0);
    // should we return true?
    return false;
}

void SandboxCamera::calcTranslation(float dt) {
    if (length(move) <= 0.1) {
        translationVector = vec3(0);
        return;
    }

    mat4 rotMatrix = mat4_cast(quat(rotation));
    vec3 forward = rotMatrix * vec4(0, 0, 1, 0);
    vec3 right = rotMatrix * vec4(1, 0, 0, 0);

    translationVector =
        (move.x * right) + (move.y * vec3(0, 1, 0)) + (move.z * forward);

    translationVector = normalize(translationVector) * speed * dt;
}

void SandboxCamera::calcRotation(float dt) {
    rotationVector += rotate * dt * mouseSpeed;

    if (rotationVector.x > 1.5) {
        rotationVector.x = 1.5;
    } else if (rotationVector.x < -1.5) {
        rotationVector.x = -1.5;
    }

    rotate = vec2(0, 0);
}

void SandboxCamera::UpdateCam() {
    static float lastTime = 0;
    float deltaTime = float(glfwGetTime() - lastTime);

    calcRotation(deltaTime);
    rotation = vec3(rotationVector.x, rotationVector.y, 0);

    calcTranslation(deltaTime);
    position += translationVector;

    lastTime = glfwGetTime();
}

bool SandboxCamera::OnEvent(Event &e) {
    EventDispatcher ed(e);
    ed.dispatch<KeyPressedEvent>(W_BIND_EVENT_FN(SandboxCamera::OnKeyPressed));
    ed.dispatch<KeyReleasedEvent>(W_BIND_EVENT_FN(SandboxCamera::OnKeyReleased));
    ed.dispatch<MouseMovedEvent>(W_BIND_EVENT_FN(SandboxCamera::OnMouseMove));
    return false; // should be irrelevant
}
