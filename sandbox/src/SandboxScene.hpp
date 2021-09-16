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

class SandboxScene : public Worlds::Scene {
  public:
    SandboxScene() {
        SetCamera(std::make_unique<Worlds::Camera>());
        GetCamera()->SetPosition(glm::vec3(0, 0, 10));

        auto cube = registry.create();
        registry.emplace<Worlds::Transform>(cube);
        registry.emplace<Worlds::Mesh>(
            cube, Worlds::CreateScope<Worlds::Cube>(glm::vec3(1.0)));
    }

  private:
};
