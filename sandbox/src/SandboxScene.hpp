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

        auto cube = registry.create();
        registry.emplace<Worlds::Transform>(cube);
        registry.emplace<Worlds::Mesh>(
            cube, Worlds::CreateScope<Worlds::Cube>(glm::vec3(1.0)));
        

        auto cube2 = registry.create();
        auto& cube2t = registry.emplace<Worlds::Transform>(cube2);
        registry.emplace<Worlds::Mesh>(
            cube2, Worlds::CreateScope<Worlds::Cube>(glm::vec3(1.0)));

        cube2t.SetPosition(glm::vec3(0, 2, 0));
    }

  private:
};
