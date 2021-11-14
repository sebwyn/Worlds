#pragma once

#include <bitset>
#include <unordered_map>
#include <vector>

#include "Worlds/Scenes/Camera.hpp"
#include "Worlds/Scenes/Scenes.hpp"

#include "Worlds/Maths/Transform.hpp"
#include "Worlds/Meshes/Mesh.hpp"
#include "Worlds/Models/Model.hpp"
#include "Worlds/Models/Shapes/Cube.hpp"

#include "SandboxCamera.hpp"

class SandboxScene : public Worlds::Scene {
  public:
    SandboxScene() {
        SetCamera(std::make_unique<Worlds::SandboxCamera>());
        camera->SetPosition(glm::vec3(0, 0, -10));

        for (int x = 0; x < 10; x++) {
            auto cube = registry.create();
            auto &transform = registry.emplace<Worlds::Transform>(cube);
            registry.emplace<Worlds::Mesh>(
                cube, Worlds::CreateScope<Worlds::Cube>(glm::vec3(1.0)));

            transform.SetPosition(glm::vec3(0, x * 2, 0));
        }
    }

  private:
};
